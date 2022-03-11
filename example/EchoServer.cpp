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
    EventLoopConcept &listenerLoop;
    std::unique_ptr<EventLoop> connLoop;
    TcpListener listener;
    TcpConnectionSet connections;

public:
    EchoServer(EventLoopConcept &loop, const InetAddress& address)
            : listenerLoop{loop},
              connections{},
              listener{loop, address} {

        std::thread{[this] {
            connLoop = std::make_unique<EventLoop>();
            connLoop->Execute();
        }}.detach();

        Coroutine::Spawn(loop, [this] {
            return listenerTask();
        });
    }

    ~EchoServer() {
        connLoop->Shutdown();
    }

private:
    Coroutine listenerTask() {
        while (true) {
            auto sock = co_await listener.Accept();
            if (sock == nullptr) {
                co_return;
            } else {
                auto &conn = connections.Emplace(listenerLoop, std::move(sock));

                Coroutine::Spawn(*connLoop, [this, &conn] {
                    return connectionTask(conn);
                }, [this, &conn] {
                    connections.Remove(conn);
                });
            }
        }
    }

    Coroutine connectionTask(TcpConnection &conn) {
        char buffer[4096];

        while (true) {
            ssize_t len = co_await conn.ReadSome(buffer, sizeof(buffer));
            if (len <= 0) { co_return ; }

            if (co_await conn.Write(buffer, len) < 0) { co_return; }
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