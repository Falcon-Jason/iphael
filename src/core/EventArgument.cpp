/**
  * @file EventArgument.cpp
  * @author Jason
  * @date 2022/3/4
  */

#include "core/EventArgument.h"

namespace iphael {
    Event::SingleBufferArgument::SingleBufferArgument(void *buffer, size_t length)
            : buffer{static_cast<char *>(buffer)},
              length{length}, lengthR{0} {
    }

    Event::SingleBufferArgument::~SingleBufferArgument() = default;

    Event::MultiBufferArgument::MultiBufferArgument() = default;

    Event::MultiBufferArgument::~MultiBufferArgument() = default;

    void Event::Argument::Set(std::nullptr_t) {
        content.emplace<nullptr_t>();
    }

    void Event::Argument::Set(void *buffer, size_t length) {
        content = std::make_unique<SingleBufferArgument>(buffer, length);
    }

    ssize_t Event::Argument::GetReturnedLength() {
        if (auto *c = Get<SingleBufferArgument>(); c != nullptr) {
            return c->lengthR;
        } else {
            return -1;
        }
    }
}