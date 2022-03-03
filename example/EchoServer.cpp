/**
  * @file EchoServer.cpp
  * @author jason
  * @date 2022/2/28
  */
#include "core/EventLoop.h"
#include "net/InetAddress.h"
#include "coroutine/TcpConnection.h"
#include "coroutine/TcpServer.h"

using namespace iphael;

coroutine::Task ConnectionTask(coroutine::TcpConnection &conn) {
    char buffer[4096];

    while (true) {
        ssize_t len = co_await conn.Read(buffer, sizeof(buffer));
        co_await conn.Write(buffer, len);
    }
}

int main() {
    EventLoop loop;

    coroutine::TcpServer server{ConnectionTask};
    server.Start(loop, InetAddress{1234});

    std::thread{[&] {
        getchar();
        loop.Shutdown();
    }}.detach();

    return loop.Execute();
}