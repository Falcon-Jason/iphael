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
    using EventHandler = std::function<void(EventMode)>;

    /**
     * The event to be handled by [class EventLoop]; which references a file descriptor instead of owning it.
     * It supports handling only one mode (read/write) at the same time.
     */
    class Event : Noncopyable {
    public:

    private:
        EventLoopConcept *parent;
        int fildes;
        int index;
        EventHandler handler;
        EventModeSet available;
        EventModeSet enabled;
        std::unique_ptr<EventPromise> promise[event_mode::SIZE];

    public:
        /**
         * Create a new event.\n
         * By default, this event's mode is EMPTY, and it has no handler.\n
         * After created, this event's mode and handler should be set.\n
         * @param loop the parent(aka. owner) event listenerLoop of this event.
         * @param fildes the file descriptor handled by this event.
         */
        Event(EventLoopConcept &loop, int fildes, EventModeSet availableModes);

        ~Event();

        /**
         * call the handler to handleEvent this event.
         */
        void Handle(EventMode mode) {
            enabled[mode] = false;
            if (handler) { handler(mode); }
        }

        /**
         * @return the file descriptor of this event.
         */
        NODISCARD int Fildes() const {
            return fildes;
        }

        NODISCARD EventModeSet AvailableModes() const {
            return available;
        }

        NODISCARD EventModeSet EnabledModes() const {
            return enabled;
        }

        /**
         * @return the parent(aka. owner) event listenerLoop of this event.
         */
        EventLoopConcept &ParentLoop() {
            return *parent;
        }

        /**
         * Enables the [param mode] and provide no buffers to it. \n
         * This event will be waiting for [param mode] triggered and call handler() immediately. \n
         * Call handler when event of [param mode] triggered. \n
         */
        void EnableAsyncEvent(EventMode mode);

        /**
         * Enables the [param mode] and provide a promise to it.\n
         * @param mode the mode of event (read or write) to be enabled.\n
         * @param buffer the place sending or receiving data.\n
         * (if mode == read) where the received data saved.\n
         * (if mode == write) the data to be sent.\n
         * @param length the length of promise.\n
         * @param strict when the handler() called.\n
         *     (if strict == true) the handler will be called after all data of [param length] sent/received. \n
         *     (if strict == false) the handler will be called after data of any length sent/received.\n
         */
        void EnableAsyncEvent(EventMode mode, void *buffer, size_t length, bool strict);

        /**
         * Set the handler of this event.
         * @param value the new handler to be set.
         * @return the reference to this, for chain setting.
         * @note no copy operation when passing an rvalue as promise.
         */
        Event &SetHandler(EventHandler value) {
            this->handler = std::move(value);
            return *this;
        }

        /**
         * @note
         *     only available for event module.
         */
        int &Index() {
            return index;
        }

        /**
         * @note
         *     only available for event module.
         */
        EventPromise *Promise(EventMode mode) {
            return promise[mode].get();
        }

        /**
         * update this event to event listenerLoop
         * @note
         *     call this function after finished handling.
         */
        void Update();
    };
} // iphael
