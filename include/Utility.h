/**
  * @file Util.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include <functional>
#include <thread>
#include <cstdlib>
#define NODISCARD [[nodiscard]]

namespace iphael {
    using Function = std::function<void()>;

    class Noncopyable {
    public:
        Noncopyable(const Noncopyable &) = delete;

        void operator=(const Noncopyable &) = delete;

    protected:
        Noncopyable() = default;

        ~Noncopyable() = default;
    };

    inline void Fetal(const char *info) {
        fprintf(stderr, "%s\n", info);
        exit(EXIT_FAILURE);
    }
} // iphael
