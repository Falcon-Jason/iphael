/**
  * @file Argument.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include "core/Event.h"
#include <memory>

namespace iphael {
    class EventArgument {
    public:
        friend class Event;
        friend class EventLoop;

    private:
        char *buffer{nullptr};
        size_t length{0};
        ssize_t lengthR{0};
        bool strict{false};

    public:
        EventArgument() = default;

        EventArgument(nullptr_t) : EventArgument{} {}

        EventArgument(void *buffer, size_t length, bool strict) {
            Set(buffer, length, strict);
        }

        bool operator==(nullptr_t) const {
            return buffer == nullptr;
        }

        /**
         * Set this as null
         */
        void Set(std::nullptr_t) { return Set(nullptr, 0, false); }

        /**
         * Set this as a single buffer buffer,
         * for read, write operations
         * @param buffer the data to be read/written
         * @param length the length of buffer
         */
        void Set(void *buf, size_t len, bool st) {
            buffer = (char *)buf;
            length = len;
            lengthR = 0;
            strict = st;
        }

        void *Buffer() { return buffer; }

        NODISCARD size_t Length() const { return length; }

        NODISCARD ssize_t ReturnedLength() const { return lengthR; }
    };
} // iphael
