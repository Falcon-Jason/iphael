/**
 * @file ThreadPool.cpp
 * @author Jason
 * @date 2022/3/12
 */

#include "event/ThreadPool.h"

#include <cassert>
#include "event/EventLoop.h"
namespace iphael {
    ThreadPool::ThreadPool(EventLoopConcept &parentLoop)
            : parentLoop{&parentLoop},
              threads{},
              threadsCount{0},
              nextThread{0},
              started{false}{
    }

    ThreadPool::~ThreadPool() {
        if (started) {
            Stop();
        }
    }

    void ThreadPool::SetThreadsCount(int count) {
        assert(!started);
        threadsCount = count;
    }

    void ThreadPool::Start() {
        assert(!started);

        loops.reserve(threadsCount);
        for(int i = 0; i < threadsCount; i++) {
            threads.emplace_back(std::thread{
               [this, i]{
                   auto loop = std::make_unique<EventLoop>();
                   loops[i] = loop.get();
                   loop->Execute();
               }
            });
        }

        started = true;
    }

    void ThreadPool::Stop() {
        assert(started);

        for (int i = 0; i < threadsCount; i++) {
            loops[i]->Shutdown();
            threads[i].detach();
        }

        loops.clear();
        threads.clear();
        started = false;
    }

    EventLoop &ThreadPool::NextLoop() {
        int ret = nextThread;
        nextThread = (nextThread + 1) % threadsCount;
        return *loops[ret];
    }
} // iphael