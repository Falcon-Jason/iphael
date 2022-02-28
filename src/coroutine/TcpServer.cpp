/**
  * @file TcpServer.cpp
  * @author jason
  * @date 2022/2/28
  */

#include "coroutine/TcpServer.h"

namespace iphael::coroutine {
    TcpConnection &TcpServer::addConnection(TcpConnection conn) {
        int fildes = conn.Fildes();
        connectionSet.emplace(std::make_pair(fildes, std::move(conn)));

        fmt::print("Added connection {}\n", fildes);
        return connectionSet.at(fildes);
    }

    void TcpServer::removeConnection(TcpConnection &conn) {
        int fildes = conn.Fildes();
        connectionSet.erase(fildes);

        fmt::print("Removed connection {}\n", fildes);
    }

    void TcpServer::handleMessage(TcpConnection &conn) {
        fmt::print("Handling connection {}\n", conn.Fildes());

        auto *arg = get_if<SingleBufferArgument>(&conn.event->Argument());
        if (arg == nullptr || arg->lengthR > 0) {
            conn.coroutine.Resume();
        } else {
            removeConnection(conn);
        }
    }

    void TcpServer::handleConnection(TcpSocket socket) {
        auto &conn = addConnection(TcpConnection{*this->parentLoop, std::move(socket)});
        conn.SetHandler([this, &conn] { handleMessage(conn); });
        task(conn);
    }
} // iphael