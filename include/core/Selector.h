/**
  * @file Selector.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once
#include "Utility.h"
#include <memory>

namespace iphael {
    class Event;

    class Selector : Noncopyable {
    private:
        int poller;
        int eventCount;

    public:
        /**
         * Create a selector.
         */
        Selector();

        /**
         * Destroy a selector.
         */
        ~Selector() noexcept;

        /**
         * AsyncWait until an event triggered
         * @return the triggered event
         */
        Event *Wait();

        /**
         * UpdateEvent the changes of event
         * @param event the event to be updated
         */
        void UpdateEvent(Event *event);

        /**
         * remove the event from selector
         * @param event the event to be removed
         */
        void RemoveEvent(Event *event);

        /**
         * @return whether there is no event to be selected.
         */
        NODISCARD bool Empty() const { return eventCount == 0; }
    };
} // iphael
