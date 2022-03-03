/**
  * @file TcpServer.h
  * @author jason
  * @date 2022/2/28
  */

#pragma once

#include "net/TcpSocket.h"
#include "coroutine/TcpListener.h"
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
        TcpListener listener;
        ConnectionSet connectionSet;
        TaskFunction task;

    public:
        explicit TcpServer(TaskFunction task);

        bool Start(EventLoopConcept &loop, const InetAddress &address) ;

        void Stop();

        NODISCARD auto Started() const { return listener.Started(); }

        EventLoopConcept &ParentLoop() { return listener.ParentLoop(); }

    private:
        TcpConnection &addConnection(TcpConnection conn);

        void removeConnection(TcpConnection &conn);

        void handleMessage(TcpConnection &conn);

        void handleConnection(TcpSocket socket);
    };
} // iphael
