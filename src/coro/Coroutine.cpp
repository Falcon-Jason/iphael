/**
  * @file Coroutine.cpp
  * @author jason
  * @date 2022/2/27
  */

#include "coro/Coroutine.h"

#include "event/EventLoopConcept.h"
#include <cassert>

namespace iphael::coro {
    Coroutine::Coroutine(Coroutine::Handle coroutineHandle) noexcept
            : handle{coroutineHandle} {
    }

    Coroutine::Coroutine(Coroutine &&rhs) noexcept
            : Coroutine{std::move(rhs.handle)} {
    }

    Coroutine &Coroutine::operator=(Coroutine::Handle &&coroutineHandle) noexcept {
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
        auto h = handle;
        handle = nullptr;

        if (auto &loop = h.promise().loop; loop != nullptr) {
            loop->RunInLoop([handle = h] { handle.resume(); });
        } else {
            h.resume();
        }
    }

#define FORWARD(name) name = std::move(name)
    void Coroutine::Spawn(EventLoopConcept &loop, const Task& task, Function afterReturned) {
        auto coro = task();
        auto &promise = coro.handle.promise();
        promise.loop = &loop;
        promise.afterReturned = std::move(afterReturned);
        coro.Resume();
    }
#undef FORWARD
} // iphael::coro