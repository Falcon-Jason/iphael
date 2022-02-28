/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/13
  */

#include "core/Event.h"
#include "core/EventLoop.h"

namespace iphael {
    Event::Event(EventLoop &loop, int fildes)
            : parent{&loop},
              fildes{fildes},
              mode{EventMode::NO_EVENT},
              handler{nullptr},
              index{-1},
              argument{nullptr} {
    }

    Event::~Event() {
        if (index >= 0) {
            parent->RemoveEvent(this);
        }
    }

    void Event::Update() {
        return parent->UpdateEvent(this);
    }

    Event &Event::SetAsyncWait(EventMode mode) {
        this->mode = mode;
        this->argument = nullptr;
        return *this;
    }

    Event &Event::SetAsyncReadSome(void *buffer, size_t length) {
        this->mode = EventMode::ASYNC_READ;
        this->argument.emplace<SingleBufferArgument>(buffer, length);
        return *this;
    }

    Event &Event::SetAsyncWrite(void *buffer, size_t length) {
        this->mode = EventMode::ASYNC_WRITE;
        this->argument.emplace<SingleBufferArgument>(buffer, length);
        return *this;
    }


}