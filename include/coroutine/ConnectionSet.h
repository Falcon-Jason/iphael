/**
 * @file ConnectionSet.h
 * @author Jason
 * @date 2022/3/8
 */

#pragma once
#include "net/TcpSocket.h"
#include <map>
#include <memory>

namespace iphael {
    class EventLoopConcept;
}

namespace iphael::coroutine {
    class TcpConnection;

    class ConnectionSet : Noncopyable {
    private:
        std::map<int, std::unique_ptr<TcpConnection>> content;

    public:
        ConnectionSet() = default;

        ~ConnectionSet();

        TcpConnection& Emplace(EventLoopConcept &loop, TcpSocket sock);

        void Remove(int fildes);

        void Remove(TcpConnection& conn);

        void Clear();
    };
} // iphael