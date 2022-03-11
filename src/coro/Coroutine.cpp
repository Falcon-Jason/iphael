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

    Coroutine::Handle Coroutine::Release() {
        auto h = handle;
        handle = nullptr;
        return h;
    }

    void Coroutine::Resume() {
        auto h = Release();

        if (auto &loop = h.promise().loop; loop != nullptr) {
            loop->RunInLoop([handle = h] { handle.resume(); });
        } else {
            h.resume();
        }
    }

    void Coroutine::Spawn(EventLoopConcept &loop, const Coroutine::Function& func, iphael::Function afterReturned) {
        auto coro = func();

        auto &promise = coro.handle.promise();
        promise.loop = &loop;
        promise.afterReturned = std::move(afterReturned);

        promise.loop->RunInLoop([ch = coro.Release()] () mutable {
            Coroutine{ch}.Resume();
        });
    }
} // iphael::coro