/**
  * @file TcpListener.cpp
  * @author Jason
  * @date 2022/3/4
  */

#include "coroutine/TcpListener.h"
#include "core/Event.h"
#include "core/EventLoop.h"
#include "net/TcpSocket.h"
#include <cassert>

namespace iphael::coroutine {
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
        coroutine.Resume();
    }

    TcpListener::Awaitable TcpListener::Accept() {
        event->SetAsyncWait(EventMode::READ);
        return Awaitable{this};
    }

    TcpListener::Awaitable::Awaitable(TcpListener *listener)
            : listener{listener} {
    }

    void TcpListener::Awaitable::await_suspend(std::coroutine_handle<> handle) {
        listener->coroutine = std::move(handle);
        listener->event->Update();
    }

    TcpSocket TcpListener::Awaitable::await_resume() const noexcept {
        return TcpSocket::Accept(listener->socket);
    }
}
