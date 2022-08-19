/**
  * @file Stream.h
  * @author jason
  * @date 2022/2/21
  */

#pragma once
#include <cstddef>
#include "Utility.h"

namespace iphael {
    class InetAddress;

    /**
     * @class Stream
     * RAII encapsulation of stream file descriptors.
     */
    class TcpSocket : Noncopyable {
    protected:
        int fildes{-1};

    public:
        TcpSocket() = default;

        TcpSocket(std::nullptr_t) : TcpSocket{} {}

        TcpSocket(TcpSocket &&rhs) noexcept : TcpSocket{rhs.Release()} {}

        TcpSocket &operator=(TcpSocket &&rhs) noexcept;

        ~TcpSocket() { Close(); }

        /**
         * Create a socket from @param fildes
         * This will move the ownership of @param fildes to created socket object.
         */
        explicit TcpSocket(int fildes) : fildes{fildes} {}

        /**
         * Close this socket and reset this to null.
         */
        void Close();

        /**
         * Release the ownership of file descriptor.
         * @return the released file descriptor, should be close by user.
         */
        [[nodiscard]] int Release();

        /**
         * @return whether this socket is null
         */
        bool operator==(std::nullptr_t) const { return fildes < 0; }

        /**
         * @return whether this socket is not null
         */
        explicit operator bool() const { return *this != nullptr; }

        /**
         * @return the file descriptor owned by this socket
         */
        int Fildes() const { return fildes; }

        /**
         * Enable/Disable TCP_NODELAY
         * (Disable/Enable Nagle's algorithm).
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetNoDelay(bool on);

        /**
         * Enable/Disable SO_REUSEADDR
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetReuseAddr(bool on);

        /**
         * Enable/Disable SO_REUSEPORT
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetReusePort(bool on);

        /**
         * Enable/Disable SO_KEEPALIVE
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetKeepAlive(bool on);

        /**
         * Enable/Disable SOCK_NONBLOCK
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetNonBlock(bool on);

        /**
         * Enable/Disable SOCK_CLOEXEC
         * @return the reference to this (for chain settings).
         */
        TcpSocket &SetCloseExec(bool on);

    private:
        TcpSocket &SetSocketOption(int option, bool on);

    public:
        /**
         * Connect to @param address and create a socket of connection.
         * @return the socket created.
         */
        static TcpSocket Connect(const InetAddress &address);

        /**
         * Bind and listen to @param address and create a socket of listener.
         * @return the socket created.
         */
        static TcpSocket Listen(const InetAddress &address);

        /**
         * Accept a socket from @param listener.
         * @return the socket of newly accepted connection.
         * @note @param listener should be a listener instead of connection socket.
         */
        static TcpSocket Accept(TcpSocket &listener);
    };
} // iphael
