/**
  * @file Connection.h
  * @author jason
  * @date 2022/2/15
  */

#pragma once

#include "core/Event.h"
#include "net/TcpSocket.h"
#include "coroutine/Task.h"
#include "coroutine/Coroutine.h"

namespace iphael {
    class InetAddress;
}

namespace iphael::coroutine {
    class TcpConnection {
    public:
        class Awaitable;

        friend class TcpServer;

    protected:
        TcpSocket socket{nullptr};
        std::unique_ptr<Event> event{nullptr};
        Coroutine coroutine{nullptr};

    public:
        TcpConnection(EventLoopConcept &loop, int fildes)
                : TcpConnection{loop, TcpSocket{fildes}} {
        }

        TcpConnection(EventLoopConcept &loop, TcpSocket socket);

        TcpConnection(TcpConnection &&rhs) noexcept = default;

        ~TcpConnection();

        void SetHandler(Function handler) {
            event->SetHandler(std::move(handler));
        }

        int Fildes() { return socket.Fildes(); }

        Awaitable Read(void *buffer, size_t length);

        Awaitable Write(void *buffer, size_t length);
    };

    class TcpConnection::Awaitable {
        TcpConnection *conn;

        explicit Awaitable(TcpConnection *conn);

    public:
        friend class TcpConnection;

        static bool await_ready() { return false; }

        NODISCARD ssize_t await_resume() const noexcept;

        void await_suspend(std::coroutine_handle<> handle);
    };

    using TcpConnectionPtr = std::unique_ptr<TcpConnection>;
} // iphael
