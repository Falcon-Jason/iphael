/**
  * @file EventLoop.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "event/EventLoop.h"

#include <cassert>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include "event/Selector.h"
#include "event/EventPromise.h"
#include "event/Event.h"

namespace iphael {
    EventLoop::EventLoop()
                : executing{false},
                  threadId{std::this_thread::get_id()},
                  selector{new Selector{}},
                  wakeupFildes{-1},
                  wakeupEvent{nullptr} {
        wakeupFildes = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
        if (wakeupFildes < 0) { return; }

        wakeupEvent = std::make_unique<Event>(*this, wakeupFildes);
        wakeupEvent->SetHandler([this] {
            handleWakeup();
            wakeupEvent->Update();
        });
    }

    EventLoop::~EventLoop() {
        close(wakeupFildes);
    }

    void EventLoop::UpdateEvent(Event *event) {
        RunInLoop([this, event] {
            selector->UpdateEvent(event);
        });
    }

    void EventLoop::RemoveEvent(Event *event) {
        RunInLoop([this, event] {
            selector->RemoveEvent(event);
        });
    }

    int EventLoop::Execute() {
        assert(wakeupFildes >= 0);
        assert(executing == false);
        assert(InLoopThread());

        executing = true;
        wakeupEvent->EnableAsyncEvent(EventMode::READ);

        while (executing) {
            Event *event = selector->Wait();
            if (event == nullptr) { continue; }

            if (processEvent(event)) {
                event->Handle();
            } else {
                selector->UpdateEvent(event);
            }

            if (selector->Empty()) { executing = false; }
        }
        return 0;
    }

    void EventLoop::RunInLoop(Function function) {
        if (InLoopThread()) {
            function();
        } else {
            queue(std::move(function));
        }
    }

    void EventLoop::queue(Function function) {
        LockGuard guard{mutex};
        submittedFunctions.emplace_back(std::move(function));
        wakeup();
    }

    void EventLoop::handleWakeup() {
        eventfd_t value;
        eventfd_read(wakeupFildes, &value);
        assert(value != 0);

        auto functions = [this] {
            LockGuard guard{mutex};
            return std::move(submittedFunctions);
        }();

        for(auto &func : functions) {
            func();
        }
    }

    void EventLoop::wakeup() {
        eventfd_write(wakeupFildes, 1);
    }

    bool EventLoop::processRead(Event *event) {
        assert(InLoopThread());

        auto *arg = event->Argument();
        if (arg == nullptr) { return true; }

        ssize_t len = read(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        arg->lengthR += len;
        if (!arg->strict || len <= 0) { return true; }
        return (arg->lengthR == arg->length);
    }

    bool EventLoop::processWrite(Event *event) {
        assert(InLoopThread());

        auto *arg = event->Argument();
        if (arg == nullptr) { return true; }

        ssize_t len = write(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        arg->lengthR += len;
        if (!arg->strict || len <= 0) { return true; }
        return (arg->lengthR == arg->length);
    }

    bool EventLoop::processEvent(Event *event) {
        if (*event->Argument() == nullptr) {
            return true;
        }

        switch (event->Mode()) {
            case EventMode::READ:
                return processRead(event);
            case EventMode::WRITE:
                return processWrite(event);
            default:
                // event must be either READ or WRITE
                assert(false);
        }
    }
} // iphael