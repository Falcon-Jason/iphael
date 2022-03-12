/**
  * @file Connection.cpp
  * @author jason
  * @date 2022/2/15
  */

#include "net/TcpConnection.h"

#include "event/EventPromise.h"
#include "event/EventLoop.h"
#include <cassert>
#include <fmt/format.h>
#include <sys/socket.h>

namespace iphael {
    TcpConnection::TcpConnection(EventLoopConcept &loop, TcpSocket socket)
            : socket{std::move(socket)},
              event{new Event{loop, this->socket.Fildes(), event_mode::RD_WR}},
              awaiter{nullptr} {
        event->SetHandler([this] (EventMode m) { handleEvent(m); } );
    }

    TcpConnection::~TcpConnection() = default;

    void TcpConnection::handleEvent(EventMode mode) {
        ParentLoop().RunInLoop([this, mode] {
                awaiter[mode].Resume();
        });
    }

    TcpConnection::Awaitable TcpConnection::ReadSome(void *buffer, size_t length) {
        return Awaitable{this, buffer, length, EventMode::READ, false};
    }

    TcpConnection::Awaitable TcpConnection::WriteSome(void *buffer, size_t length) {
        return Awaitable{this, buffer, length, EventMode::WRITE, false};
    }

    TcpConnection::Awaitable TcpConnection::Read(void *buffer, size_t length) {
        return Awaitable{this, buffer, length, EventMode::READ, true};
    }

    TcpConnection::Awaitable TcpConnection::Write(void *buffer, size_t length) {
        return Awaitable{this, buffer, length, EventMode::WRITE, true};
    }

    /// Functions for Awaitable

    TcpConnection::Awaitable::Awaitable(
            TcpConnection *conn, void *buffer, size_t length,
            EventMode mode, bool useStrict)
            : conn{conn},
              buffer{buffer},
              length{length},
              mode{mode},
              useStrict{useStrict} {
    }

    void TcpConnection::Awaitable::await_suspend(Coroutine::Handle handle) {
        conn->ParentLoop().RunInLoop( [this, handle] () mutable {
            conn->awaiter[mode] = handle;
            conn->event->EnableAsyncEvent(mode, buffer, length, useStrict);
        });
    }

    ssize_t TcpConnection::Awaitable::await_resume() const noexcept {
        auto len = conn->event->Promise(mode)->ReturnedLength();
        conn->event->Promise(mode)->Set(nullptr);
        return len;
    }
} // iphael