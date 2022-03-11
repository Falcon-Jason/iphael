/**
  * @file EventLoopConcept.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once

namespace iphael {
    class Event;

    class EventLoopConcept {
    public:
        virtual ~EventLoopConcept() = default;

        /**
         * Update and activate the event.
         * Call this function before being listened or after modified.
         * @param event the event to be updated.
         */
        virtual void UpdateEvent(Event *event) = 0;

        /**
         * remove the given event from current event-loop.
         * Should be called before the event is destroyed.
         * @param event the event to be removed
         */
        virtual void RemoveEvent(Event *event) = 0;

        /**
         * Run @param function in the thread owned by this loop.
         * If caller is running at the same thread, call @param function directly.
         * Otherwise, send @param function to this and wakeup.
         */
        virtual void RunInLoop(Function function) = 0;
    };
} // iphael
