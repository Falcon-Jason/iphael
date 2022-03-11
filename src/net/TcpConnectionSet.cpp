/**
 * @file TcpConnectionSet.cpp
 * @author Jason
 * @date 2022/3/8
 */

#include "net/TcpConnectionSet.h"

#include <fmt/format.h>
#include "net/TcpConnection.h"

namespace iphael {
    TcpConnectionSet::~TcpConnectionSet() {
        Clear();
    }

    TcpConnection &TcpConnectionSet::Emplace(EventLoopConcept &loop, TcpSocket sock) {
        auto fildes = sock.Fildes();
        fmt::print("Emplaced connection {}\n", fildes);

        auto connPtr = std::make_unique<TcpConnection>(loop, std::move(sock));
        connPtr->SetErrorHandler([this, fildes] { Remove(fildes); } );

        content.emplace(fildes, std::move(connPtr));
        return *content.at(fildes);
    }

    void TcpConnectionSet::Remove(int fildes) {
        content.erase(fildes);
        fmt::print("Removed connection {}\n", fildes);
    }

    void TcpConnectionSet::Remove(TcpConnection &conn) {
        return Remove(conn.Fildes());
    }

    void TcpConnectionSet::Clear() {
        while (!content.empty()) {
            Remove(content.begin()->first);
        }
    }
} // iphael