/**
  * @file Connection.cpp
  * @author jason
  * @date 2022/2/15
  */

#include "../core/EventArgument.h"
#include "coroutine/TcpConnection.h"
#include "coroutine/TcpServer.h"
#include <sys/socket.h>
#include <cassert>
#include <fmt/format.h>

namespace iphael::coroutine {
    TcpConnection::TcpConnection(EventLoopConcept &loop, TcpSocket socket)
            : socket{std::move(socket)},
              event{new Event{loop, this->socket.Fildes()}},
              coroutine{nullptr} {
    }

    TcpConnection::~TcpConnection() = default;

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
        return conn->event->EventArgument().GetReturnedLength();
    }

    void TcpConnection::Awaitable::await_suspend(std::coroutine_handle<> handle) {
        conn->coroutine = std::move(handle);
        conn->event->Update();
    }
} // iphael