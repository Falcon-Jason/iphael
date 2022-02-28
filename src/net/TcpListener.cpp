/**
  * @file TcpListener.cpp
  * @author jason
  * @date 2022/2/24
  */

#include "net/TcpListener.h"

#include <cassert>

namespace iphael {
    bool TcpListener::Start(EventLoop &loop, const InetAddress &address) {
        assert(!Started());

        socket = TcpSocket::Listen(address);
        if (socket == nullptr) { return false; }

        event.reset(new Event{loop, socket.Fildes()});
        event->SetHandler([this] { onNewConnection(); });
        event->SetAsyncWait(EventMode::ASYNC_ACCEPT);
        event->Update();
        return true;
    }

    void TcpListener::Stop() {
        event = nullptr;
        socket.Close();
    }

    void TcpListener::onNewConnection() {
        callback(TcpSocket::Accept(socket));
        event->Update();
    }
} // iphael