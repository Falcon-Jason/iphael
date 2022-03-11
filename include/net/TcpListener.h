/**
  * @file TcpListener.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include <functional>
#include <memory>
#include "Utility.h"
#include "coro/Task.h"
#include "coro/Coroutine.h"
#include "net/TcpSocket.h"

namespace iphael {
    class Event;
    class EventLoopConcept;
    class InetAddress;

    class TcpListener : Noncopyable {
    public:
        class Awaitable;
        using Callback = std::function<void(TcpSocket)>;

    private:
        TcpSocket socket{nullptr};
        std::unique_ptr<Event> event{nullptr};
        coro::Coroutine coroutine{nullptr};

    public:
        TcpListener(EventLoopConcept &loop, TcpSocket sock);

        TcpListener(EventLoopConcept &loop, int fildes)
                : TcpListener{loop, TcpSocket{fildes}} {
        }

        TcpListener(EventLoopConcept& loop, const InetAddress& address)
                : TcpListener{loop, TcpSocket::Listen(address)} {
        }

        NODISCARD int Fildes() const;

        EventLoopConcept &ParentLoop();

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
} // iphael
