/**
  * @file TcpListener.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include "Utility.h"
#include "net/TcpSocket.h"
#include "coroutine/Task.h"
#include "coroutine/Coroutine.h"
#include <functional>
#include <memory>

namespace iphael {
    class Event;
    class ExecutorConcept;
    class InetAddress;

    namespace coroutine {
        class TcpListener : Noncopyable {
        public:
            class Awaitable;
            using Callback = std::function<void(TcpSocket)>;

        private:
            TcpSocket socket{nullptr};
            std::unique_ptr<Event> event{nullptr};
            Coroutine coroutine{nullptr};

        public:
            TcpListener(ExecutorConcept &loop, TcpSocket sock);

            TcpListener(ExecutorConcept &loop, int fildes)
                    : TcpListener{loop, TcpSocket{fildes}} {
            }

            TcpListener(ExecutorConcept& loop, const InetAddress& address)
                    : TcpListener{loop, TcpSocket::Listen(address)} {
            }

            NODISCARD int Fildes() const;

            ExecutorConcept &ParentLoop();

            Awaitable Accept();

        private:
            void handleEvent();
        };

        class TcpListener::Awaitable {
        private:
            TcpListener *listener;

            Awaitable(TcpListener *listener);

        public:
            friend class TcpListener;

            NODISCARD bool await_ready() const { return false; }

            NODISCARD TcpSocket await_resume() const noexcept;

            void await_suspend(std::coroutine_handle<> handle);
        };
    } // coroutine
} // iphael
