/**
  * @file Stream.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once
#include "Utility.h"
#include <variant>

namespace iphael {
    enum class EventMode : int16_t {
        READ,
        WRITE,
        END,
        EMPTY = END,
    };

    class EventLoopConcept;
    class EventArgument;

    /**
     * @class Event
     * The event to be handled by @class EventLoop; which references a file descriptor instead of owning it.
     * It supports handling only one mode (read/write) at the same time.
     */
    class Event : Noncopyable {
    public:

    private:
        EventLoopConcept *parent;
        int fildes;
        EventMode mode;
        Function handler;
        int index;
        std::unique_ptr<EventArgument> buffer;

    public:
        /**
         * Create a new event.
         * By default, this event's mode is EMPTY, and it has no handler.
         * After created, this event's mode and handler should be set.
         * @param loop the parent(aka. owner) event loop of this event.
         * @param fildes the file descriptor handled by this event.
         */
        Event(EventLoopConcept &loop, int fildes);

        ~Event();

        /**
         * call the handler to handleEvent this event.
         */
        void Handle() {
            if (handler) { handler(); }
        }

        /**
         * @return the file descriptor of this event.
         */
        NODISCARD int Fildes() const {
            return fildes;
        }

        /**
         * @return the IO mode of this event.
         */
        NODISCARD EventMode Mode() const {
            return mode;
        }

        /**
         * @return the parent(aka. owner) event loop of this event.
         */
        EventLoopConcept &ParentLoop() {
            return *parent;
        }

        /**
         * Set the m of event to reactor-style async wait.
         * Call handler when event of @param mode triggered.
         */
        void SetAsyncWait(EventMode mode);

        /**
         * Set the mode of event to async read.
         * Call handler when any length of data received.
         * @param buffer where the read data saved
         * @param length the length of buffer
         */
        void SetAsyncReadSome(void *buffer, size_t length);

        /**
         * Set the mode of event to async read.
         * Call handler when all data received.
         * @param buffer where the read data saved
         * @param length the length of buffer
         */
        void SetAsyncRead(void *buffer, size_t length);

        /**
         * Set the mode of event to async write.
         * Call handler when any length of data received.
         * @param buffer the data to be sent
         * @param length the length of buffer.
         */
        void SetAsyncWriteSome(void *buffer, size_t length);

        /**
         * Set the mode of event to async write.
         * Call handler when all data sent.
         * @param buffer the data to be sent
         * @param length the length of buffer.
         */
        void SetAsyncWrite(void *buffer, size_t length);

        /**
         * Set the mode of this event.
         * @param value the mode to be set.
         * @return the reference to this, for chain setting.
         */
        Event &SetMode(EventMode value) {
            this->mode = value;
            return *this;
        }

        /**
         * Set the handler of this event.
         * @param value the new handler to be set.
         * @return the reference to this, for chain setting.
         * @note no copy operation when passing an rvalue as buffer.
         */
        Event &SetHandler(Function value) {
            this->handler = std::move(value);
            return *this;
        }

        /**
         * @note only available for core classes.
         */
        int &Index() {
            return index;
        }

        /**
         * @note only available for core classes.
         */
        EventArgument *Argument() {
            return buffer.get();
        }

        /**
         * update this event to event loop
         * @note call this function after finished handling.
         */
        void Update();
    };
} // iphael
