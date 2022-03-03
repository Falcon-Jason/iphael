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
              ioMode{IOMode::EMPTY},
              bufferMode{BufferMode::NO_BUFFER},
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

    void Event::SetAsyncWait(IOMode m) {
        ioMode = m;
        bufferMode = BufferMode::AWAITING;
        argument->Set(nullptr);
    }

    void Event::SetAsyncReadSome(void *buffer, size_t length) {
        ioMode = IOMode::READ;
        bufferMode = BufferMode::SINGLE_BUFFER;
        argument->Set(buffer, length);
    }

    void Event::SetAsyncWrite(void *buffer, size_t length) {
        this->ioMode = IOMode::WRITE;
        bufferMode = BufferMode::SINGLE_BUFFER;
        argument->Set(buffer, length);
    }
}