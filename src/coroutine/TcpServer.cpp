/**
  * @file TcpServer.cpp
  * @author jason
  * @date 2022/2/28
  */
#include "core/EventBuffer.h"
#include "coroutine/TcpServer.h"

namespace iphael::coroutine {
    TcpServer::TcpServer(ExecutorConcept &loop, const InetAddress &address)
            : loop{loop},
              connectionSet{},
              listener{loop, address} {
        listenerTask();
    }

    TcpServer::~TcpServer() {
        if (!connectionSet.empty()) {
            for (auto &it : connectionSet) {
                Remove(it.first);
            }
        }
    }

    Task TcpServer::listenerTask() {
        while (true) {
            auto sock = co_await listener.Accept();
            if (sock == nullptr) {
                continue;
            } else {
                connectionTask(Emplace(std::move(sock)));
            }
        }
    }

    TcpConnection &TcpServer::Emplace(TcpSocket socket) {
        auto fildes = socket.Fildes();
        fmt::print("Emplaced connection {}\n", fildes);

        auto connPtr = std::make_unique<TcpConnection>(loop, std::move(socket));
        connPtr->SetErrorHandler([this, fildes] { Remove(fildes); } );

        connectionSet.emplace(fildes, std::move(connPtr));
        return *connectionSet.at(fildes);
    }

    void TcpServer::Remove(int fildes) {
        connectionSet.erase(fildes);
        fmt::print("Removed connection {}\n", fildes);
    }
} // iphael