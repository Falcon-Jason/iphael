/**
  * @file Coroutine.h
  * @author jason
  * @date 2022/2/27
  */

#pragma once

#include "Utility.h"
#include <coroutine>

namespace iphael::coroutine {
    /**
     * @class Coroutine
     * RAII encapsulation of coroutineHandler handleEvent.
     */
    class Coroutine : Noncopyable {
    private:
        std::coroutine_handle<> handle{nullptr};

    public:
        Coroutine() = default;

        Coroutine(nullptr_t) : Coroutine{} {}

        explicit Coroutine(std::coroutine_handle<> &&handle) noexcept;

        Coroutine(Coroutine &&rhs) noexcept;

        ~Coroutine() { Destroy(); }

        Coroutine &operator=(std::coroutine_handle<> &&handle) noexcept;

        Coroutine &operator=(Coroutine &&rhs) noexcept;

        Coroutine &operator=(nullptr_t) noexcept;

        constexpr bool operator==(nullptr_t) const {
            return handle == nullptr;
        }

        constexpr operator bool() const {
            return *this != nullptr;
        }

        constexpr auto *Address() const {
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
    };
} // iphael
