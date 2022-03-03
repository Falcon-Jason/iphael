/**
  * @file Stream.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once
#include "Utility.h"
#include <variant>

namespace iphael {
    enum class IOMode : int16_t {
        READ,
        WRITE,
        END,
        EMPTY = END,
    };

    enum class BufferMode : int16_t {
        AWAITING,
        SINGLE_BUFFER,
        MULTI_BUFFER,
        END,
        NO_BUFFER = AWAITING
    };

    constexpr int operator&&(IOMode ioMode, BufferMode bufferMode) {
        return (int)(ioMode) * (int)(IOMode::END) + (int)(bufferMode);
    }

    class ExecutorConcept;

    /**
     * @class Event
     * The event to be handled by @class EventLoop; which references a file descriptor instead of owning it.
     * It supports handling only one ioMode (read/write) at the same time.
     */
    class Event : Noncopyable {
    public:
        class Argument;
        struct SingleBufferArgument;
        struct MultiBufferArgument;

    private:
        ExecutorConcept *parent;
        int fildes;
        IOMode ioMode;
        BufferMode bufferMode;
        Function handler;
        int index;
        std::unique_ptr<Argument> argument;

    public:
        /**
         * Create a new event.
         * By default, this event's ioMode is EMPTY, and it has no handler.
         * After created, this event's ioMode and handler should be set.
         * @param loop the parent(aka. owner) event loop of this event.
         * @param fildes the file descriptor handled by this event.
         */
        Event(ExecutorConcept &loop, int fildes);

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
        NODISCARD int Fildes() const {
            return fildes;
        }

        /**
         * @return the IO ioMode of this event.
         */
        NODISCARD IOMode Mode() const {
            return ioMode;
        }

        NODISCARD BufferMode BufMode() const {
            return bufferMode;
        }

        /**
         * @return the parent(aka. owner) event loop of this event.
         */
        ExecutorConcept &ParentLoop() {
            return *parent;
        }

        /**
         * Set the m of event to reactor-style async wait.
         * Call handler when event of @param m triggered.
         */
        void SetAsyncWait(IOMode m);

        /**
         * Set the ioMode of event to async read.
         * Call handler when any length of data received
         * @param buffer where the read data saved
         * @param length the length of buffer
         */
        void SetAsyncReadSome(void *buffer, size_t length);

        /**
         * Set the ioMode of event to async write.
         * Call handler when all data sent.
         * @param buffer the data to be sent
         * @param length the length of buffer.
         */
        void SetAsyncWrite(void *buffer, size_t length);

        /**
         * Set the ioMode of this event.
         * @param value the ioMode to be set.
         * @return the reference to this, for chain setting.
         */
        Event &SetMode(IOMode value) {
            this->ioMode = value;
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
        Argument &EventArgument() {
            return *argument;
        }

        /**
         * update this event to event loop
         * @note call this function after finished handling.
         */
        void Update();
    };
} // iphael
