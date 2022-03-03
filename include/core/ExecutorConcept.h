/**
  * @file ExecutorConcept.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once

namespace iphael {
    class Event;

    class ExecutorConcept {
    public:
        virtual ~ExecutorConcept() = default;

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
} // iphael
