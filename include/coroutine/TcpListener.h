/**
  * @file TcpListener.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include "Utility.h"
#include "coroutine/Task.h"
#include "coroutine/Coroutine.h"
#include <functional>
#include <memory>

namespace iphael {
    class Event;
    class EventLoopConcept;
    class InetAddress;
    class TcpSocket;

    namespace coroutine {
        class TcpListener : Noncopyable {
        public:
            class Awaitable;
            using Callback = std::function<void(TcpSocket)>;

        private:
            std::unique_ptr<TcpSocket> socket{nullptr};
            std::unique_ptr<Event> event{nullptr};
            Coroutine coroutine{nullptr};
            Callback callback{nullptr};

        public:
            explicit TcpListener(Callback cb)
                    : callback{std::move(cb)} {
            }

            bool Start(EventLoopConcept& loop, const InetAddress& address);

            void Stop();

            NODISCARD bool Started() const {
                return socket != nullptr;
            }

            NODISCARD int Fildes() const;

            EventLoopConcept &ParentLoop();

            Awaitable Accept();

        private:
            Task main();

            void handleEvent();
        };

        class TcpListener::Awaitable {
        private:
            TcpListener *listener;

            Awaitable(TcpListener *listener);

        public:
            friend class TcpListener;

            bool await_ready() const { return false; }

            NODISCARD TcpSocket await_resume() const noexcept;

            void await_suspend(std::coroutine_handle<> handle);
        };
    } // coroutine
} // iphael
