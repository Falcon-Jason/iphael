/**
  * @file TcpListener.cpp
  * @author Jason
  * @date 2022/3/4
  */

#include "net/TcpListener.h"

#include <cassert>
#include "event/Event.h"
#include "event/EventLoop.h"
#include "net/TcpSocket.h"

namespace iphael {
    TcpListener::TcpListener(EventLoopConcept &loop, TcpSocket sock)
            : socket{std::move(sock)},
              event{new Event{loop, socket.Fildes()}} {
        event->SetHandler([this] { handleEvent(); } );
    }

    int TcpListener::Fildes() const {
        return socket.Fildes();
    }

    EventLoopConcept &TcpListener::ParentLoop() {
        return event->ParentLoop();
    }

    void TcpListener::handleEvent() {
        ParentLoop().RunInLoop([this] { coroutine.Resume(); });
    }

    TcpListener::Awaitable TcpListener::Accept() {
        return Awaitable{this};
    }

    TcpListener::Awaitable::Awaitable(TcpListener *listener)
            : listener{listener} {
    }

    void TcpListener::Awaitable::await_suspend(std::coroutine_handle<> handle) {
        listener->ParentLoop().RunInLoop([this, handle] () mutable {
            listener->coroutine = std::move(handle);
            listener->event->EnableAsyncEvent(EventMode::READ);
        });
    }

    TcpSocket TcpListener::Awaitable::await_resume() const noexcept {
        return TcpSocket::Accept(listener->socket);
    }
} // iphael
