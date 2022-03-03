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
    bool TcpListener::Start(EventLoopConcept &loop, const InetAddress &address) {
        assert(!Started());

        socket = std::make_unique<TcpSocket>(TcpSocket::Listen(address));
        if (socket == nullptr || *socket == nullptr) {
            return false;
        }

        event = std::make_unique<Event>(loop, socket->Fildes());
        event->SetHandler([this] { handleEvent(); });

        main();
        return true;
    }

    void TcpListener::Stop() {
        event = nullptr;
        socket->Close();
    }

    int TcpListener::Fildes() const {
        return socket == nullptr ? -1 : socket->Fildes();
    }

    EventLoopConcept &TcpListener::ParentLoop() {
        return event->ParentLoop();
    }

    void TcpListener::handleEvent() {
        if (coroutine) {
            coroutine.Resume();
        }
    }

    Task TcpListener::main() {
        while (true) {
            auto sock = co_await Accept();
            if (sock == nullptr) {
                Stop();
                co_return;
            } else {
                callback(std::move(sock));
            }
        }
    }

    TcpListener::Awaitable TcpListener::Accept() {
        event->SetAsyncWait(EventMode::ASYNC_ACCEPT);
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
        return TcpSocket::Accept(*listener->socket);
    }
}
