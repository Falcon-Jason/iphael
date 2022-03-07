/**
  * @file TcpServer.cpp
  * @author jason
  * @date 2022/2/28
  */
#include <map>
#include <fmt/format.h>
#include "core/Signal.h"
#include "core/EventLoop.h"
#include "net/InetAddress.h"
#include "coroutine/TcpConnection.h"
#include "coroutine/TcpListener.h"
#include "coroutine/TcpServer.h"

using namespace iphael;
using namespace iphael::coroutine;


int main() {
    EventLoop loop;

    TcpServer server(loop, InetAddress{1234});

    server.SetConnectionTask(
            [] (TcpConnection &conn) -> Task {
                char buffer[4096];

                while (true) {
                    ssize_t len = co_await conn.Read(buffer, sizeof(buffer));
                    co_await conn.Write(buffer, len);
                }
            });

    std::thread([&loop] {
        getchar();
        loop.Shutdown();
    }).detach();

//    Signal(SIGTERM, [&loop] { loop.Shutdown(); } );
    return loop.Execute();
}