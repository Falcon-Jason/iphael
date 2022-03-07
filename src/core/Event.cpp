/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "core/Event.h"
#include "core/EventArgument.h"
#include "core/EventLoop.h"

namespace iphael {

    Event::Event(EventLoopConcept &loop, int fildes)
            : parent{&loop},
              fildes{fildes},
              mode{EventMode::EMPTY},
              handler{nullptr},
              index{-1},
              buffer{new EventArgument{}} {
    }

    Event::~Event() {
        if (index >= 0) {
            parent->RemoveEvent(this);
        }
    }

    void Event::Update() {
        return parent->UpdateEvent(this);
    }

    void Event::SetAsyncWait(EventMode m) {
        mode = m;
        buffer->Set(nullptr);
    }

    void Event::SetAsyncReadSome(void *buf, size_t len) {
        mode = EventMode::READ;
        buffer->Set(buf, len, false);
    }

    void Event::SetAsyncRead(void *buf, size_t len) {
        mode = EventMode::READ;
        buffer->Set(buf, len, true);
    }

    void Event::SetAsyncWriteSome(void *buf, size_t len) {
        mode = EventMode::WRITE;
        buffer->Set(buf, len, false);
    }

    void Event::SetAsyncWrite(void *buf, size_t len) {
        mode = EventMode::WRITE;
        buffer->Set(buf, len, true);
    }
}