/**
  * @file Util.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <cstdlib>
#define NODISCARD [[nodiscard]]

namespace iphael {
    using Function = std::function<void()>;
    using LockGuard = std::lock_guard<std::mutex>;

    class Noncopyable {
    public:
        Noncopyable(const Noncopyable &) = delete;

        void operator=(const Noncopyable &) = delete;

        Noncopyable(Noncopyable&&) noexcept = default;

        Noncopyable &operator=(Noncopyable &&rhs) noexcept = default;

    protected:
        Noncopyable() = default;

        ~Noncopyable() = default;
    };

    inline void Fetal(const char *info) {
        fprintf(stderr, "%s\n", info);
        exit(EXIT_FAILURE);
    }
} // iphael
