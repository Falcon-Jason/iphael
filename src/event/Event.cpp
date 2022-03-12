/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "event/Event.h"

#include "event/EventPromise.h"
#include "event/EventLoop.h"

namespace iphael {
    Event::Event(EventLoopConcept &loop, int fildes)
            : parent{&loop},
              fildes{fildes},
              mode{EventMode::EMPTY},
              handler{nullptr},
              index{-1},
              promise{new EventPromise{}} {
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
        mode = m;
        promise->Set(nullptr);
        Update();
    }

    void Event::EnableAsyncEvent(EventMode m, void *buf, size_t len, bool useStrict) {
        mode = m;
        promise->Set(buf, len, useStrict);
        Update();
    }
} // iphael