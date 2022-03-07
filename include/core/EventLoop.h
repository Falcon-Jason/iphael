/**
  * @file IOProcessor.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once
#include "Utility.h"
#include "core/EventLoopConcept.h"
#include <atomic>
#include <thread>
#include <memory>
#include <deque>
#include <mutex>

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
    class EventLoop : public EventLoopConcept {
    protected:
        bool executing;
        std::thread::id threadId;
        std::unique_ptr<Selector> selector;
        int wakeupFildes;
        std::unique_ptr<Event> wakeupEvent;
        std::mutex mutex;
        std::deque<Function> submittedFunctions; /// @guarded_by mutex

    public:
        /**
         * Create a default event loop
         */
        EventLoop();

        ~EventLoop() override;

        /**
         * @return whether current thread is bind to this loop.
         */
        bool InLoopThread() {
            return threadId == std::this_thread::get_id();
        }

        /**
         * Run @param function in the thread owned by this loop.
         * If caller is running at the same thread, call @param function directly.
         * Otherwise, send @param function to this and wakeup.
         */
        void RunInLoop(Function function);

        /**
         * start running the event loop.
         * @return the result after terminated, has the same meaning with return value of main function.
         */
        int Execute();

        /**
         * Stop running the event-loop.
         * @note thread safe: thread safe: it is available to be called in other threads.
         */
        void Shutdown() {
            RunInLoop([this] { executing = false; } );
        }

        /**
         * Update and activate the event.
         * Call this function before being listened or after modified.
         * @param event the event to be updated.
         * @note thread safe: it is available to be called in other threads.
         */
        void UpdateEvent(Event *event) override;

        /**
         * remove the given event from current event-loop.
         * Should be called before the event is destroyed.
         * @param event the event to be removed.
         * @note thread safe: it is available to be called in other threads.
         */
        void RemoveEvent(Event *event) override;

    private:
        void handleWakeup();

        void wakeup();

        void queue(Function function);

        bool processEvent(Event *event);

        bool processRead(Event *event);

        bool processWrite(Event *event);
    };
} // iphael
