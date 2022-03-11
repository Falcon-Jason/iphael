/**
  * @file Coroutine.h
  * @author jason
  * @date 2022/2/27
  */

#pragma once

#include "Utility.h"
#include <coroutine>

namespace iphael {
    class EventLoopConcept;
}

namespace iphael::coro {
    /**
     * @class Coroutine
     * RAII encapsulation of coroutineHandler handleEvent.
     */
    class Coroutine : Noncopyable {
    public:
        class Promise;

        using Handle = std::coroutine_handle<Promise>;

        using Task = std::function<Coroutine()>;

        using promise_type = Promise;

    private:
        Handle handle{nullptr};

    public:
        Coroutine() = default;

        Coroutine(nullptr_t) : Coroutine{} {}

        explicit Coroutine(Handle handle) noexcept;

        Coroutine(Coroutine &&rhs) noexcept;

        ~Coroutine() { Destroy(); }

        Coroutine &operator=(Handle &&handle) noexcept;

        Coroutine &operator=(Coroutine &&rhs) noexcept;

        Coroutine &operator=(nullptr_t) noexcept;

        constexpr bool operator==(nullptr_t) const {
            return handle == nullptr;
        }

        constexpr operator bool() const {
            return *this != nullptr;
        }

        NODISCARD constexpr auto *Address() const {
            return handle.address();
        }

        /**
         * Resume and released this coroutineHandler.
         * @note this will be null after resumed.
         */
        void Resume();

        /**
         * Destroy this coroutineHandler.
         * @note this will be null after resumed.
         */
        void Destroy();

        static void Spawn(EventLoopConcept &loop, const Task& task, Function afterReturned = nullptr);
    };

    class Coroutine::Promise {
    public:
        friend class Coroutine;

    private:
        iphael::Function afterReturned{};
        EventLoopConcept *loop;

    public:
        auto get_return_object() {
            return Coroutine{Handle::from_promise(*this)};
        }

        auto initial_suspend() noexcept { return std::suspend_always{}; }

        auto final_suspend() noexcept { return std::suspend_never{}; }

        void unhandled_exception() { std::terminate(); }

        void return_void() {
            if (afterReturned != nullptr) { afterReturned(); }
        }
    };
} // iphael::coro
