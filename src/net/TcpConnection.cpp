/**
  * @file Connection.cpp
  * @author jason
  * @date 2022/2/15
  */

#include "net/TcpConnection.h"

#include "event/EventArgument.h"
#include "event/EventLoop.h"
#include <cassert>
#include <fmt/format.h>
#include <sys/socket.h>

namespace iphael {
    TcpConnection::TcpConnection(EventLoopConcept &loop, TcpSocket socket)
            : socket{std::move(socket)},
              event{new Event{loop, this->socket.Fildes()}},
              coroutine{nullptr} {
        event->SetHandler([this] { handleEvent(); } );
    }

    TcpConnection::~TcpConnection() = default;

    void TcpConnection::handleEvent() {
        ParentLoop().RunInLoop([this] {
            if (event->Argument()->ReturnedLength() <= 0) {
                errorHandler();
            } else {
                coroutine.Resume();
            }
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

    void TcpConnection::Awaitable::await_suspend(std::coroutine_handle<> handle) {
        conn->ParentLoop().RunInLoop( [this, handle] () mutable {
            conn->coroutine = std::move(handle);
            conn->event->EnableAsyncEvent(mode, buffer, length, useStrict);
        });
    }

    ssize_t TcpConnection::Awaitable::await_resume() const noexcept {
        auto len = conn->event->Argument()->ReturnedLength();
        conn->event->Argument()->Set(nullptr);
        return len;
    }
} // iphael