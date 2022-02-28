/**
  * @file Stream.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include "Utility.h"
#include "EventArgument.h"

namespace iphael {
    enum class EventMode {
        ASYNC_READ,
        ASYNC_WRITE,
        ASYNC_ACCEPT,
        NO_EVENT
    };

    class EventLoop;

    class Event : Noncopyable {
    private:
        EventLoop *parent;
        int fildes;
        EventMode mode;
        Function handler;
        int index;
        EventArgument argument;

    public:
        /**
         * Create a new event.
         * By default, this event's mode is NO_EVENT, and it has no handler.
         * After created, this event's mode and handler should be set.
         * @param loop the parent(aka. owner) event loop of this event.
         * @param fildes the file descriptor handled by this event.
         */
        Event(EventLoop &loop, int fildes);

        ~Event();

        /**
         * call the handler to handle this event.
         */
        void Handle() {
            if (handler) { handler(); }
        }

        /**
         * @return the file descriptor of this event.
         */
        int Fildes() const {
            return fildes;
        }

        /**
         * @return the mode of this event.
         */
        EventMode Mode() const {
            return mode;
        }

        /**
         * @return the parent(aka. owner) event loop of this event.
         */
        EventLoop &ParentLoop() {
            return *parent;
        }

        /**
         * Set the mode of event to reactor-style async wait.
         * Call handler when event of @param mode triggered.
         * @return
         */
        Event &SetAsyncWait(EventMode mode);

        /**
         * Set the mode of event to async read.
         * Call handler when any length of data received
         * @param buffer where the read data saved
         * @param length the length of buffer
         * @return
         */
        Event &SetAsyncReadSome(void *buffer, size_t length);

        /**
         * Set the mode of event to async write.
         * Call handler when all data sent.
         * @param buffer the data to be sent
         * @param length the length of buffer.
         * @return
         */
        Event &SetAsyncWrite(void *buffer, size_t length);

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
         * @note no copy operation when passing an rvalue as argument.
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
        EventArgument &Argument() {
            return argument;
        }

        /**
         * update this event to event loop
         * @note call this function after finished handling.
         */
        void Update();
    };
} // iphael
