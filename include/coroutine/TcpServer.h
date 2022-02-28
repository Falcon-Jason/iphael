/**
  * @file TcpServer.h
  * @author jason
  * @date 2022/2/28
  */

#pragma once

#include "net/TcpListener.h"
#include "net/TcpSocket.h"
#include "coroutine/TcpConnection.h"
#include <map>
#include <functional>
#include <memory>
#include <variant>
#include <fmt/format.h>

namespace iphael::coroutine {
    class TcpServer {
    public:
        using ConnectionSet = std::map<int, TcpConnection>;
        using TaskFunction = std::function<Task(TcpConnection & )>;

    private:
        EventLoop *parentLoop{};
        TcpListener listener{};
        ConnectionSet connectionSet{};
        TaskFunction task{nullptr};

    public:
        explicit TcpServer(TaskFunction task) {
            this->task = std::move(task);
            this->listener.SetCallback(std::bind(
                    &TcpServer::handleConnection, this,
                    std::placeholders::_1));
        }

        auto Start(EventLoop &loop, const InetAddress &address) {
            parentLoop = &loop;
            return listener.Start(loop, address);
        }

        void Stop() {
            listener.Stop();
            parentLoop = nullptr;
        }

        auto Started() const { return listener.Started(); }

    private:
        TcpConnection &addConnection(TcpConnection conn);

        void removeConnection(TcpConnection &conn);

        void handleMessage(TcpConnection &conn);

        void handleConnection(TcpSocket socket);
    };
} // iphael
