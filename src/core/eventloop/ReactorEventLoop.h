/**
  * @file MultiplexProcessor.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include "core/EventLoop.h"
#include "selector/Selector.h"
#include <memory>

namespace iphael {
    class ReactorEventLoop : public EventLoop {
    private:
        SelectorPtr selector;

    public:
        explicit ReactorEventLoop()
                : selector{MakeDefaultSelector()} {
        }

        explicit ReactorEventLoop(SelectorPtr selector)
                : selector{std::move(selector)} {
        }

        int Execute() override;

        void UpdateEvent(Event *event) override;

        void RemoveEvent(Event *event) override;

    private:
        bool ProcessEvent(Event *event);

        bool ProcessRead(Event *event);

        bool ProcessWrite(Event *event);
    };
} // iphael
