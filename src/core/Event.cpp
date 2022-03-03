/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "core/Event.h"
#include "core/EventArgument.h"
#include "core/EventLoop.h"

namespace iphael {
    Event::Event(ExecutorConcept &loop, int fildes)
            : parent{&loop},
              fildes{fildes},
              mode{EventMode::NO_EVENT},
              handler{nullptr},
              index{-1},
              argument{new Argument{}} {
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
        argument->Set(nullptr);
    }

    void Event::SetAsyncReadSome(void *buffer, size_t length) {
        mode = EventMode::ASYNC_READ;
        argument->Set(buffer, length);
    }

    void Event::SetAsyncWrite(void *buffer, size_t length) {
        this->mode = EventMode::ASYNC_WRITE;
        argument->Set(buffer, length);
    }
}