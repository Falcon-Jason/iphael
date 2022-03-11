/**
  * @file Connection.h
  * @author jason
  * @date 2022/2/15
  */

#pragma once
#include "event/Event.h"
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

    public:
        TcpConnection(EventLoopConcept &loop, int fildes)
                : TcpConnection{loop, TcpSocket{fildes}} {
        }

        TcpConnection(EventLoopConcept &loop, TcpSocket socket);

        TcpConnection(TcpConnection &&rhs) noexcept = default;

        ~TcpConnection();

        int Fildes() { return socket.Fildes(); }

        EventLoopConcept &ParentLoop() { return event->ParentLoop(); }

        Awaitable ReadSome(void *buffer, size_t length);

        Awaitable Read(void *buffer, size_t length);

        Awaitable WriteSome(void *buffer, size_t length);

        Awaitable Write(void *buffer, size_t length);

    private:
        void handleEvent();
    };

    // created and running in coroutine's event listenerLoop
    class TcpConnection::Awaitable {
        TcpConnection *conn;
        void *buffer;
        size_t length;
        EventMode mode;
        bool useStrict;

        explicit Awaitable(
                TcpConnection *conn, void *buffer, size_t length,
                EventMode mode, bool useStrict);

    public:
        friend class TcpConnection;

        NODISCARD bool await_ready() { return false; }

        NODISCARD ssize_t await_resume() const noexcept;

        void await_suspend(coro::Coroutine::Handle handle);
    };

    using TcpConnectionPtr = std::unique_ptr<TcpConnection>;
} // iphael
