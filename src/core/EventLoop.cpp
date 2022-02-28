/**
  * @file EventLoop.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "core/EventLoop.h"
#include "eventloop/ReactorEventLoop.h"

namespace iphael {
    EventLoopPtr MakeDefaultEventLoop() {
        return EventLoopPtr{ new ReactorEventLoop{} };
    }
}