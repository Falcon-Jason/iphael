/**
 * @file ThreadPool.h
 * @author Jason
 * @date 2022/3/12
 */

#pragma once
#include <vector>
#include <thread>

namespace iphael {
    class EventLoopConcept;
    class EventLoop;

    class ThreadPool {
    private:
        EventLoopConcept *parentLoop;
        std::vector<std::thread> threads;
        std::vector<EventLoop *> loops;
        int threadsCount;
        int nextThread;
        bool started;

    public:
        explicit ThreadPool(EventLoopConcept &parentLoop);

        ~ThreadPool();

        void SetThreadsCount(int count);

        void Start();

        void Stop();

        EventLoop &NextLoop();
    };
} // iphael