/**
  * @file Connection.h
  * @author jason
  * @date 2022/2/15
  */

#pragma once
#include "event/Event.h"
#include "coro/Task.h"
#include "coro/Coroutine.h"
#include "net/TcpSocket.h"

namespace iphael {
    class InetAddress;

    class TcpConnection : Noncopyable {
    public:
        class Awaitable;

        friend class TcpServer;

    private:
        TcpSocket socket{nullptr};
        std::unique_ptr<Event> event{nullptr};
        coro::Coroutine coroutine{nullptr};
        Function errorHandler{nullptr};

    public:
        TcpConnection(EventLoopConcept &loop, int fildes)
                : TcpConnection{loop, TcpSocket{fildes}} {
        }

        TcpConnection(EventLoopConcept &loop, TcpSocket socket);

        TcpConnection(TcpConnection &&rhs) noexcept = default;

        ~TcpConnection();

        void SetErrorHandler(Function handler) {
            errorHandler = std::move(handler);
        }

        int Fildes() { return socket.Fildes(); }

        Awaitable Read(void *buffer, size_t length);

        Awaitable Write(void *buffer, size_t length);

    private:
        void handleEvent();
    };

    class TcpConnection::Awaitable {
        TcpConnection *conn;

        explicit Awaitable(TcpConnection *conn);

    public:
        friend class TcpConnection;

        NODISCARD bool await_ready() { return false; }

        NODISCARD ssize_t await_resume() const noexcept;

        void await_suspend(std::coroutine_handle<> handle);
    };

    using TcpConnectionPtr = std::unique_ptr<TcpConnection>;
} // iphael
