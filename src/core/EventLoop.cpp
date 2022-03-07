/**
  * @file EventLoop.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "core/Selector.h"
#include "core/EventArgument.h"
#include "core/Event.h"
#include "core/EventLoop.h"
#include <cassert>
#include <sys/socket.h>

namespace iphael {
    EventLoop::EventLoop()
                : executing{false},
                  threadId{std::this_thread::get_id()},
                  selector{new Selector{}} {
    }

    EventLoop::~EventLoop() = default;

    void EventLoop::UpdateEvent(Event *event) {
        return selector->UpdateEvent(event);
    }

    void EventLoop::RemoveEvent(Event *event) {
        return selector->RemoveEvent(event);
    }

    int EventLoop::Execute() {
        assert(executing == false);
        assert(InLoopThread());

        executing = true;
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
}