/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "core/Event.h"
#include "core/EventBuffer.h"
#include "core/EventLoop.h"

namespace iphael {

    Event::Event(ExecutorConcept &loop, int fildes)
            : parent{&loop},
              fildes{fildes},
              mode{EventMode::EMPTY},
              handler{nullptr},
              index{-1},
              buffer{new Buffer{}} {
    }

    Event::~Event() {
        if (index >= 0) {
            parent->RemoveEvent(this);
        }
    }

    EventBufferMode Event::BufferMode() {
        return buffer ? buffer->mode : EventBufferMode::NOT_SUPPORTED;
    }

    void Event::Update() {
        return parent->UpdateEvent(this);
    }

    void Event::SetAsyncWait(EventMode m) {
        mode = m;
        buffer->mode = EventBufferMode::AWAITING;
        buffer->Set(nullptr);
    }

    void Event::SetAsyncReadSome(void *buf, size_t len) {
        mode = EventMode::READ;
        buffer->mode = EventBufferMode::SINGLE_BUFFER;
        buffer->Set(buf, len);
    }

    void Event::SetAsyncWrite(void *buf, size_t len) {
        this->mode = EventMode::WRITE;
        buffer->mode = EventBufferMode::SINGLE_BUFFER;
        buffer->Set(buf, len);
    }
}