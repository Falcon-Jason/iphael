/**
  * @file EventLoop.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "Selector.h"
#include "EventArgument.h"
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

            if (event->EventArgument() == nullptr || processEvent(event)) {
                event->Handle();
            } else {
                selector->UpdateEvent(event);
            }

            if (selector->Empty()) { executing = false; }
        }
        return 0;
    }

    bool EventLoop::processEvent(Event *event) {
        switch (event->Mode()) {
            case EventMode::ASYNC_READ:
                return processRead(event);
            case EventMode::ASYNC_WRITE:
                return processWrite(event);
            default:
                return true;
        }
    }

    bool EventLoop::processRead(Event *event) {
        assert(InLoopThread());

        auto *arg = event->EventArgument().Get<Event::SingleBufferArgument>();
        if (arg == nullptr) { return true; }

        arg->lengthR = read(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        return true;
    }

    bool EventLoop::processWrite(Event *event) {
        assert(InLoopThread());

        auto *arg = event->EventArgument().Get<Event::SingleBufferArgument>();
        if (arg == nullptr) { return true; }

        ssize_t len = write(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        if (len <= 0) { return true; }
        arg->lengthR += len;
        return (arg->lengthR == arg->length);
    }
}