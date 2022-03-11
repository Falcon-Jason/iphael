/**
  * @file Connection.cpp
  * @author jason
  * @date 2022/2/15
  */

#include "net/TcpConnection.h"

#include "event/EventArgument.h"
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
        if (event->Argument()->ReturnedLength() <= 0) {
            errorHandler();
        } else {
            coroutine.Resume();
        }
    }

    TcpConnection::Awaitable TcpConnection::Read(void *buffer, size_t length) {
        event->SetAsyncReadSome(buffer, length);
        return Awaitable{this};
    }

    TcpConnection::Awaitable TcpConnection::Write(void *buffer, size_t length) {
        event->SetAsyncWrite(buffer, length);
        return Awaitable{this};
    }

    /// Functions for Awaitable

    TcpConnection::Awaitable::Awaitable(TcpConnection *conn) {
        this->conn = conn;
    }

    ssize_t TcpConnection::Awaitable::await_resume() const noexcept {
        auto len = conn->event->Argument()->ReturnedLength();
        conn->event->Argument()->Set(nullptr);
        return len;
    }

    void TcpConnection::Awaitable::await_suspend(std::coroutine_handle<> handle) {
        conn->coroutine = std::move(handle);
        conn->event->Update();
    }
} // iphael