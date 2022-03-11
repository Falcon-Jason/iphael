/**
  * @file Signal.cpp
  * @author jason
  * @date 2022/2/27
  */

#include "event/Signal.h"

#include <csignal>
#include <map>

namespace iphael {
    static std::map<int, std::function<void()>> SignalHandlerSet{};

    void Signal(int signalNumber, std::function<void()> signalHandler) {
        SignalHandlerSet.emplace(signalNumber, std::move(signalHandler));
        signal(signalNumber, [](int s) { SignalHandlerSet.at(s)(); });
    }
} // iphael