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
              event{new Event{loop, socket.Fildes(), event_mode::RD_ONLY}} {
        event->SetHandler([this] (EventMode m) {
            assert(m == event_mode::READ);
            handleEvent();
        } );
    }

    EventLoopConcept &TcpListener::ParentLoop() {
        return event->ParentLoop();
    }

    void TcpListener::handleEvent() {
        ParentLoop().RunInLoop([this] {
            coroutine.Resume();
        });
    }

    TcpListener::Awaitable TcpListener::Accept() {
        return Awaitable{this};
    }

    TcpListener::Awaitable::Awaitable(TcpListener *listener)
            : listener{listener} {
    }

    void TcpListener::Awaitable::await_suspend(Coroutine::Handle handle) {
        listener->ParentLoop().RunInLoop([this, handle] () mutable {
            listener->coroutine = handle;
            listener->event->EnableAsyncEvent(EventMode::READ);
        });
    }

    TcpSocket TcpListener::Awaitable::await_resume() const noexcept {
        return TcpSocket::Accept(listener->socket);
    }
} // iphael
