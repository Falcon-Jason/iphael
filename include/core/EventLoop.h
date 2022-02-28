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

    class EventLoop {
    protected:
        std::atomic_bool executing;
        std::thread::id threadId;

    public:
        EventLoop()
                : executing{false},
                  threadId{std::this_thread::get_id()} {
        }

        virtual ~EventLoop() = default;

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
        virtual int Execute() = 0;

        /**
         * Stop running the event-loop.
         */
        void Shutdown() { executing = false; }

        /**
         * Update and activate the event.
         * Call this function before being listened or after modified.
         * @param event the event to be updated.
         */
        virtual void UpdateEvent(Event *event) = 0;

        /**
         * Remove the given event from current event-loop.
         * Should be called before the event is destroyed.
         * @param event the event to be removed
         */
        virtual void RemoveEvent(Event *event) = 0;
    };

    using EventLoopPtr = std::unique_ptr<EventLoop>;

    /**
     * static factory function for creating event-loop.
     * @return a newly created event-loop.
     */
    EventLoopPtr MakeDefaultEventLoop();
} // iphael
