/**
 * @file TcpConnectionSet.h
 * @author Jason
 * @date 2022/3/8
 */

#pragma once
#include <map>
#include <memory>
#include "net/TcpSocket.h"

namespace iphael {
    class TcpConnection;

    class EventLoopConcept;

    class TcpConnectionSet : Noncopyable {
    private:
        std::map<int, std::unique_ptr<TcpConnection>> content;

    public:
        TcpConnectionSet() = default;

        ~TcpConnectionSet();

        TcpConnection& Emplace(EventLoopConcept &loop, TcpSocket sock);

        void Remove(int fildes);

        void Remove(TcpConnection& conn);

        void Clear();
    };
} // iphael