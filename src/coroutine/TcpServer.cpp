/**
  * @file TcpServer.cpp
  * @author jason
  * @date 2022/2/28
  */
#include "../core/EventArgument.h"
#include "coroutine/TcpServer.h"

namespace iphael::coroutine {
    TcpServer::TcpServer(TaskFunction task)
            : listener{[this](auto sock){ handleConnection(std::move(sock)); }},
              connectionSet{},
              task{std::move(task)}{
    }

    bool TcpServer::Start(ExecutorConcept &loop, const InetAddress &address) {
        return listener.Start(loop, address);
    }

    void TcpServer::Stop()  {
        listener.Stop();
    }

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

        auto *arg = conn.event->EventArgument().Get<Event::SingleBufferArgument>();
        if (arg == nullptr || arg->lengthR > 0) {
            conn.coroutine.Resume();
        } else {
            removeConnection(conn);
        }
    }

    void TcpServer::handleConnection(TcpSocket socket) {
        auto &conn = addConnection(TcpConnection{ParentLoop(), std::move(socket)});
        conn.SetHandler([this, &conn] { handleMessage(conn); });
        task(conn);
    }
} // iphael