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
        Coroutine awaiter[event_mode::SIZE]{nullptr};

    public:
        TcpConnection(EventLoopConcept &loop, TcpSocket socket);

        TcpConnection(EventLoopConcept &loop, int fildes)
                : TcpConnection{loop, TcpSocket{fildes}} {
        }

        TcpConnection(TcpConnection &&rhs) noexcept = default;

        ~TcpConnection();

        bool operator==(std::nullptr_t) const { return socket == nullptr; }

        NODISCARD int Fildes() const { return socket.Fildes(); }

        EventLoopConcept &ParentLoop() { return event->ParentLoop(); }

        Awaitable ReadSome(void *buffer, size_t length);

        Awaitable Read(void *buffer, size_t length);

        Awaitable WriteSome(void *buffer, size_t length);

        Awaitable Write(void *buffer, size_t length);

    private:
        void handleEvent(EventMode mode);
    };

    // created and running in awaiter's event listenerLoop
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

        void await_suspend(Coroutine::Handle handle);
    };

    using TcpConnectionPtr = std::unique_ptr<TcpConnection>;
} // iphael
