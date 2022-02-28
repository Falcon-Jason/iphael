/**
  * @file Coroutine.h
  * @author jason
  * @date 2022/2/19
  */

#pragma once

#include <coroutine>

namespace iphael::coroutine {
    struct Task {
        struct promise_type {
            auto get_return_object() { return Task{}; }

            auto initial_suspend() noexcept { return std::suspend_never{}; }

            auto final_suspend() noexcept { return std::suspend_never{}; }

            void unhandled_exception() { std::terminate(); }

            void return_void() {}
        };
    };
}