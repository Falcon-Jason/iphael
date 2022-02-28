/**
  * @file Coroutine.cpp
  * @author jason
  * @date 2022/2/27
  */

#include "coroutine/Coroutine.h"
#include <cassert>

namespace iphael::coroutine {
    Coroutine::Coroutine(std::coroutine_handle<> &&coroutineHandle) noexcept
            : handle{coroutineHandle} {
        coroutineHandle = nullptr;
    }

    Coroutine::Coroutine(Coroutine &&rhs) noexcept
            : Coroutine{std::move(rhs.handle)} {
    }

    Coroutine &Coroutine::operator=(std::coroutine_handle<> &&coroutineHandle) noexcept {
        assert(handle != coroutineHandle);

        Destroy();
        handle = coroutineHandle;
        coroutineHandle = nullptr;
        return *this;
    }

    Coroutine &Coroutine::operator=(Coroutine &&rhs) noexcept {
        return *this = std::move(rhs.handle);
    }

    Coroutine &Coroutine::operator=(nullptr_t) noexcept {
        Destroy();
        return *this;
    }

    void Coroutine::Destroy() {
        if (handle != nullptr) {
            handle.destroy();
            handle = nullptr;
        }
    }

    void Coroutine::Resume() {
        auto coroutineHandle = handle;
        handle = nullptr;
        return coroutineHandle.resume();
    }
}