/**
  * @file Connection.cpp
  * @author jason
  * @date 2022/2/15
  */

#include "coroutine/TcpConnection.h"
#include "coroutine/TcpServer.h"
#include <sys/socket.h>
#include <cassert>
#include <fmt/format.h>

namespace iphael::coroutine {
    TcpConnection::TcpConnection(EventLoop &loop, TcpSocket socket)
            : socket{std::move(socket)},
              event{new Event{loop, this->socket.Fildes()}},
              coroutine{nullptr} {
    }

    TcpConnection::~TcpConnection() = default;

    TcpConnection::Awaiter TcpConnection::Read(void *buffer, size_t length) {
        event->SetAsyncReadSome(buffer, length);
        return Awaiter{this};
    }

    TcpConnection::Awaiter TcpConnection::Write(void *buffer, size_t length) {
        event->SetAsyncWrite(buffer, length);
        return Awaiter{this};
    }

    // Functions for Awaiter
    TcpConnection::Awaiter::Awaiter(TcpConnection *conn) {
        this->conn = conn;
    }

    ssize_t TcpConnection::Awaiter::await_resume() const noexcept {
        auto *arg = get_if<SingleBufferArgument>(&conn->event->Argument());
        return arg == nullptr ? -1 : arg->lengthR;
    }

    void TcpConnection::Awaiter::await_suspend(std::coroutine_handle<> handle) {
        conn->coroutine = std::move(handle);
        conn->event->Update();
    }
} // iphael