/**
  * @file Signal.cpp
  * @author jason
  * @date 2022/2/27
  */

#include "core/Signal.h"
#include <map>
#include <csignal>

namespace iphael {
    static std::map<int, std::function<void()>> SignalHandlerSet{};

    void Signal(int signalNumber, std::function<void()> signalHandler) {
        SignalHandlerSet.emplace(signalNumber, std::move(signalHandler));
        signal(signalNumber, [](int s) { SignalHandlerSet.at(s)(); });
    }
} // iphael