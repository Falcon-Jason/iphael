/**
  * @file MultiplexProcessor.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "core/Event.h"
#include "selector/Selector.h"
#include "ReactorEventLoop.h"
#include <cassert>
#include <sys/socket.h>

namespace iphael {
    void ReactorEventLoop::UpdateEvent(Event *event) {
        return selector->UpdateEvent(event);
    }

    void ReactorEventLoop::RemoveEvent(Event *event) {
        return selector->RemoveEvent(event);
    }

    int ReactorEventLoop::Execute() {
        assert(executing == false);
        assert(InLoopThread());

        executing = true;
        while (executing) {
            Event *event = selector->Wait();
            if (event == nullptr) { continue; }

            if (event->Argument().index() == 0 || ProcessEvent(event)) {
                event->Handle();
            } else {
                selector->UpdateEvent(event);
            }

            if (selector->Empty()) { executing = false; }
        }
        return 0;
    }

    bool ReactorEventLoop::ProcessEvent(Event *event) {
        switch (event->Mode()) {
            case EventMode::ASYNC_READ:
                return ProcessRead(event);
            case EventMode::ASYNC_WRITE:
                return ProcessWrite(event);
            default:
                return true;
        }
    }

    bool ReactorEventLoop::ProcessRead(Event *event) {
        assert(InLoopThread());

        auto *arg = get_if<SingleBufferArgument>(&event->Argument());
        if (arg == nullptr) { return true; }

        arg->lengthR = read(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        return true;
    }

    bool ReactorEventLoop::ProcessWrite(Event *event) {
        assert(InLoopThread());

        auto *arg = get_if<SingleBufferArgument>(&event->Argument());
        if (arg == nullptr) { return true; }

        ssize_t len = write(
                event->Fildes(),
                arg->buffer + arg->lengthR,
                arg->length - arg->lengthR);

        if (len <= 0) { return true; }
        arg->lengthR += len;
        return (arg->lengthR == arg->length);
    }
} // iphael