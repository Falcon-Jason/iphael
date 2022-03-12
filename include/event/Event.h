/**
  * @file Stream.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once
#include <bitset>
#include "Utility.h"

namespace iphael {
    namespace event_mode {
        enum Mode : int {
            READ,
            WRITE,
            END,
            EMPTY = END,
        };
        constexpr auto SIZE = Mode::END;

        using ModeSet = std::bitset<SIZE>;
        constexpr auto RD_ONLY = ModeSet{1 << READ};
        constexpr auto WR_ONLY = ModeSet{1 << WRITE};
        constexpr auto RD_WR = ModeSet{(1 << READ) | (1 << WRITE)};
    }

    class EventLoopConcept;
    class EventPromise;
    using EventMode = event_mode::Mode;
    using EventModeSet = event_mode::ModeSet;

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
        std::unique_ptr<EventPromise> promise;

    public:
        /**
         * Create a new event.
         * By default, this event's mode is EMPTY, and it has no handler.
         * After created, this event's mode and handler should be set.
         * @param loop the parent(aka. owner) event listenerLoop of this event.
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
         * @return the parent(aka. owner) event listenerLoop of this event.
         */
        EventLoopConcept &ParentLoop() {
            return *parent;
        }

        /**
         * Enables the @param mode and provide no buffers to it.
         * This event will be waiting for @param mode triggered and call handler() immediately.
         * Call handler when event of @param mode triggered.
         */
        void EnableAsyncEvent(EventMode mode);

        /**
         * Enables the @param mode and provide a promise to it.
         * @param mode the mode of event (read or write) to be enabled.
         * @param buffer the place sending or receiving data.
               (if @param mode == read) where the received data saved.
               (if @param mode == write) the data to be sent.
         * @param length the length of promise.
         * @param useStrict when the handler() called.
               (if @param useStrict == true) the handler will be called after all data of @param length sent/received.
               (if @param useStrict == false) the handler will be called after data of any length sent/received.
         */
        void EnableAsyncEvent(EventMode mode, void *buffer, size_t length, bool useStrict);


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
         * @note no copy operation when passing an rvalue as promise.
         */
        Event &SetHandler(Function value) {
            this->handler = std::move(value);
            return *this;
        }

        /**
         * @note only available for event module.
         */
        int &Index() {
            return index;
        }

        /**
         * @note only available for event module.
         */
        EventPromise *Promise() {
            return promise.get();
        }

        /**
         * update this event to event listenerLoop
         * @note call this function after finished handling.
         */
        void Update();
    };
} // iphael
