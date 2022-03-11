/**
  * @file TcpServer.cpp
  * @author jason
  * @date 2022/2/28
  */
#include <map>
#include <fmt/format.h>
#include "event/Signal.h"
#include "event/EventLoop.h"
#include "net/InetAddress.h"
#include "net/TcpConnectionSet.h"
#include "net/TcpConnection.h"
#include "net/TcpListener.h"

using namespace iphael;
using namespace iphael::coro;

class EchoServer {
private:
    EventLoopConcept &loop;
    TcpListener listener;
    TcpConnectionSet connections;

public:
    EchoServer(EventLoopConcept &loop, const InetAddress& address)
            : loop{loop},
              connections{},
              listener{loop, address} {
        listenerTask();
    }

private:
    Task listenerTask() {
        while (true) {
            auto sock = co_await listener.Accept();
            if (sock == nullptr) {
                continue;
            } else {
                auto &conn = connections.Emplace(loop, std::move(sock));
                connectionTask(conn);
            }
        }
    }

    Task connectionTask(TcpConnection &conn) {
        char buffer[4096];

        while (true) {
            ssize_t len = co_await conn.ReadSome(buffer, sizeof(buffer));
            co_await conn.Write(buffer, len);
        }
    }
};


int main() {
    EventLoop loop;
    EchoServer server{loop, InetAddress{1234}};

    std::thread([&loop] {
        getchar();
        loop.Shutdown();
    }).detach();

    return loop.Execute();
}