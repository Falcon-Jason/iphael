/**
  * @file EpollSelector.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include <map>
#include "Selector.h"

namespace iphael {
    class EpollSelector : public Selector {
    private:
        int poller;
        int eventCount;

    public:
        EpollSelector();

        ~EpollSelector() noexcept override;

        Event *Wait() override;

        void UpdateEvent(Event *event) override;

        void RemoveEvent(Event *event) override;

        bool Empty() override { return eventCount == 0; };
    };
} // iphael
