/**
  * @file IOProcessor.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include <atomic>
#include <thread>
#include <memory>

namespace iphael {
    class Event;
    class Selector;

    /**
     * class EventLoop
     * The basic implementation of event loop,
     * usually used in single-thread async model,
     * or as main event loop in one-loop-per-thread model.
     * This event loop can only run in one I/O thread.
     */
    class EventLoop {
    protected:
        std::atomic_bool executing;
        std::thread::id threadId;
        std::unique_ptr<Selector> selector;

    public:
        /**
         * Create a default event loop
         */
        EventLoop();

        ~EventLoop();

        /**
         * @return whether current thread is bind to this loop.
         */
        bool InLoopThread() {
            return threadId == std::this_thread::get_id();
        }

        /**
         * start running the event loop.
         * @return the result after terminated, has the same meaning with return value of main function.
         */
        int Execute();

        /**
         * Stop running the event-loop.
         */
        void Shutdown() { executing = false; }

        /**
         * Update and activate the event.
         * Call this function before being listened or after modified.
         * @param event the event to be updated.
         */
        void UpdateEvent(Event *event);

        /**
         * Remove the given event from current event-loop.
         * Should be called before the event is destroyed.
         * @param event the event to be removed
         */
        void RemoveEvent(Event *event);

    private:
        bool processEvent(Event *event);

        bool processRead(Event *event);

        bool processWrite(Event *event);
    };
} // iphael
