/**
 * @file ConnectionSet.cpp
 * @author Jason
 * @date 2022/3/8
 */

#include <fmt/format.h>
#include "coroutine/ConnectionSet.h"
#include "coroutine/TcpConnection.h"


namespace iphael::coroutine {
    ConnectionSet::~ConnectionSet() {
        Clear();
    }

    TcpConnection &ConnectionSet::Emplace(EventLoopConcept &loop, TcpSocket sock) {
        auto fildes = sock.Fildes();
        fmt::print("Emplaced connection {}\n", fildes);

        auto connPtr = std::make_unique<TcpConnection>(loop, std::move(sock));
        connPtr->SetErrorHandler([this, fildes] { Remove(fildes); } );

        content.emplace(fildes, std::move(connPtr));
        return *content.at(fildes);
    }

    void ConnectionSet::Remove(int fildes) {
        content.erase(fildes);
        fmt::print("Removed connection {}\n", fildes);
    }

    void ConnectionSet::Remove(TcpConnection &conn) {
        return Remove(conn.Fildes());
    }

    void ConnectionSet::Clear() {
        while (!content.empty()) {
            Remove(content.begin()->first);
        }
    }
}