/**
  * @file Stream.cpp
  * @author jason
  * @date 2022/2/21
  */

#include "net/TcpSocket.h"
#include "net/InetAddress.h"
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

namespace iphael {
    TcpSocket &TcpSocket::operator=(TcpSocket &&rhs) noexcept {
        assert(this != &rhs);

        Close();
        fildes = rhs.Release();
        return *this;
    }

    void TcpSocket::Close() {
        if (fildes >= 0) {
            close(fildes);
            fildes = -1;
        }
    }

    int TcpSocket::Release() {
        int fd = fildes;
        fildes = -1;
        return fd;
    }

    inline TcpSocket &TcpSocket::SetSocketOption(int option, bool on) {
        int optVal = on ? 1 : 0;
        setsockopt(fildes, SOL_SOCKET, option, &optVal,
                   static_cast<socklen_t>(sizeof(option)));
        return *this;
    }

    TcpSocket &TcpSocket::SetNoDelay(bool on) {
        return SetSocketOption(TCP_NODELAY, on);
    }

    TcpSocket &TcpSocket::SetReuseAddr(bool on) {
        return SetSocketOption(SO_REUSEADDR, on);
    }

    TcpSocket &TcpSocket::SetReusePort(bool on) {
        return SetSocketOption(SO_REUSEPORT, on);
    }

    TcpSocket &TcpSocket::SetKeepAlive(bool on) {
        return SetSocketOption(SO_KEEPALIVE, on);
    }

    TcpSocket &TcpSocket::SetNonBlock(bool on) {
        return SetSocketOption(SOCK_NONBLOCK, on);
    }

    TcpSocket &TcpSocket::SetCloseExec(bool on) {
        return SetSocketOption(SOCK_CLOEXEC, on);
    }

    TcpSocket TcpSocket::Connect(const InetAddress &address) {
        TcpSocket sock{};

        if (sock.fildes = socket(AF_INET, SOCK_STREAM, 0);
                sock.fildes < 0) {
            return nullptr;
        }
        if (connect(sock.fildes, address.SockAddr(), address.Size()) != 0) {
            return nullptr;
        }

        sock.SetCloseExec(true).SetNonBlock(true);
        return sock;
    }

    TcpSocket TcpSocket::Listen(const InetAddress &address) {
        TcpSocket sock{};

        if (sock.fildes = socket(AF_INET, SOCK_STREAM, 0); sock.fildes < 0) {
            return nullptr;
        }

        sock.SetReuseAddr(true);

        if (bind(sock.fildes, address.SockAddr(), address.Size()) != 0) {
            return nullptr;
        }
        if (listen(sock.fildes, 1024) != 0) {
            return nullptr;
        }

        sock.SetCloseExec(true).SetNonBlock(true);
        return sock;
    }

    TcpSocket TcpSocket::Accept(TcpSocket &listener) {
        return TcpSocket{accept4(
                listener.fildes, nullptr, nullptr,
                SOCK_NONBLOCK | SOCK_CLOEXEC)};
    }
} // iphael