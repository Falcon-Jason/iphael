/**
  * @file TcpListener.h
  * @author jason
  * @date 2022/2/24
  */

#pragma once

#include "core/Event.h"
#include "net/TcpSocket.h"
#include "coroutine/TcpConnection.h"
#include <memory>
#include <functional>

namespace iphael {
    class InetAddress;

    class TcpListener : Noncopyable {
    public:
        using Callback = std::function<void(TcpSocket sock)>;

    private:
        TcpSocket socket{nullptr};
        std::unique_ptr<Event> event{nullptr};
        Callback callback{nullptr};

    public:
        TcpListener() = default;

        explicit TcpListener(Callback callback)
                : callback{std::move(callback)} {
        }

        TcpListener &SetCallback(Callback cb) {
            callback = std::move(cb);
            return *this;
        }

        bool Start(EventLoop &loop, const InetAddress &address);

        void Stop();

        bool Started() const { return socket != nullptr; }

        EventLoop &ParentLoop() { return event->ParentLoop(); }

        int Fildes() const { return socket.Fildes(); }

    private:
        void onNewConnection();
    };
} // iphael
