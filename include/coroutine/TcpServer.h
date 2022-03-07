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
        using ConnectionSet = std::map<int, TcpConnectionPtr>;
        using ConnectionTask = std::function<Task(TcpConnection &)>;
        using ListenerTask = std::function<Task()>;

    private:
        ExecutorConcept &loop;
        ConnectionSet connectionSet;
        TcpListener listener;
        ConnectionTask connectionTask;

    public:
        TcpServer(ExecutorConcept &loop, const InetAddress& address);

        ~TcpServer();

        void SetConnectionTask(ConnectionTask task) {
            connectionTask = std::move(task);
        }

        TcpConnection &Emplace(TcpSocket socket);

        void Remove(int fildes);

    private:
        Task listenerTask();


    };
} // iphael
