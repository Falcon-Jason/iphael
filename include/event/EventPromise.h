/**
  * @file Argument.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include <memory>
#include "event/Event.h"

namespace iphael {
    class EventPromise {
    public:
        friend class Event;
        friend class EventLoop;

    private:
        char *buffer{nullptr};
        size_t length{0};
        ssize_t lengthR{0};
        bool strict{false};

    public:
        EventPromise() = default;

        EventPromise(nullptr_t) : EventPromise{} {}

        EventPromise(void *buffer, size_t length, bool strict) {
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
         * Set this as a single promise promise,
         * for read, write operations
         * @param buffer the data to be read/written
         * @param length the length of promise
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
