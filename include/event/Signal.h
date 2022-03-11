/**
  * @file Signal.h
  * @author jason
  * @date 2022/2/27
  */

#pragma once
#include <functional>
#include <csignal>

namespace iphael {
    void Signal(int signalNumber, std::function<void()> signalHandler);
} // iphael
