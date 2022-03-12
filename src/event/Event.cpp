/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "event/Event.h"

#include "event/EventPromise.h"
#include "event/EventLoop.h"

namespace iphael {
    Event::Event(EventLoopConcept &loop, int fildes, EventModeSet availableModes)
            : parent{&loop},
              fildes{fildes},
              index{-1},
              handler{nullptr},
              promise{nullptr},
              available{availableModes},
              enabled{} {
        if (availableModes[event_mode::READ]) {
            promise[event_mode::READ] = std::make_unique<EventPromise>();
        }
        if (availableModes[event_mode::WRITE]) {
            promise[event_mode::WRITE] = std::make_unique<EventPromise>();
        }
    }

    Event::~Event() {
        if (index >= 0) {
            parent->RemoveEvent(this);
        }
    }

    void Event::Update() {
        return parent->UpdateEvent(this);
    }

    void Event::EnableAsyncEvent(EventMode m) {
        enabled[m] = true;
        promise[m]->Set(nullptr);
        Update();
    }

    void Event::EnableAsyncEvent(EventMode m, void *buf, size_t len, bool strict) {
        enabled[m] = true;
        promise[m]->Set(buf, len, strict);
        Update();
    }
} // iphael