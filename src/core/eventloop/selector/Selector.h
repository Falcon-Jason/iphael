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
    public:
        virtual ~Selector() = default;

        /**
         * AsyncWait until an event triggered
         * @return the triggered event
         */
        virtual Event *Wait() = 0;

        /**
         * UpdateEvent the changes of event
         * @param event the event to be updated
         */
        virtual void UpdateEvent(Event *event) = 0;

        /**
         * Remove the event from selector
         * @param event the event to be removed
         */
        virtual void RemoveEvent(Event *event) = 0;

        virtual bool Empty() = 0;
    };

    using SelectorPtr = std::unique_ptr<Selector>;

    /**
     * static factory function for creating selector.
     * @return a newly created selector.
     */
    SelectorPtr MakeDefaultSelector();
} // iphael
