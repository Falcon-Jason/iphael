/**
  * @file EventLoop.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "core/Selector.h"
#include "core/EventBuffer.h"
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
        assert(event->EventBuffer().Mode() == EventBufferMode::SINGLE_BUFFER); // TODO: MULTI_BUFFER not implemented

        auto *arg = event->EventBuffer().Get<Event::SingleBufferArgument>();
        if (arg == nullptr) { return true; }

        arg->lengthR = read(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        return true;
    }

    bool EventLoop::processWrite(Event *event) {
        assert(InLoopThread());
        assert(event->EventBuffer().Mode() == EventBufferMode::SINGLE_BUFFER); // TODO: MULTI_BUFFER not implemented

        auto *arg = event->EventBuffer().Get<Event::SingleBufferArgument>();
        if (arg == nullptr) { return true; }

        ssize_t len = write(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        if (len <= 0) { return true; }
        arg->lengthR += len;
        return (arg->lengthR == arg->length);
    }

    bool EventLoop::processReadV(Event *event) {
        assert(false); // TODO: NOT IMPLEMENTED
    }

    bool EventLoop::processWriteV(Event *event) {
        assert(false); // TODO: NOT IMPLEMENTED
    }

    bool EventLoop::processEvent(Event *event) {
        switch (event->EventBuffer().Mode()) {
            case EventBufferMode::AWAITING: {
                // process nothing in awaiting buffer-mode.
                return true;
            }

            case EventBufferMode::SINGLE_BUFFER: {
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

            case EventBufferMode::MULTI_BUFFER: {
                switch (event->Mode()) {
                    case EventMode::READ:
                        return processReadV(event);
                    case EventMode::WRITE:
                        return processWriteV(event);
                    default:
                        // event must be either READ or WRITE
                        assert(false);
                }
            }

            default: {
                // buffer-mode must be AWAITING, SINGLE_BUFFER or MULTI_BUFFER
                assert(false);
            }
        }
    }
}