/**
  * @file InetAddress.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "net/InetAddress.h"
#include <cstring>
#include <fmt/format.h>

namespace iphael {
    InetAddress::InetAddress() : address{} {
        memset(&address, 0, Size()); // Initialize `address` by setting all zero.
        address.sin_family = AF_INET;
    }

    InetAddress::InetAddress(const char *addr, uint16_t port) : InetAddress{} {
        SetPort(port);
        SetAddress(addr);
    }

    InetAddress::InetAddress(uint16_t port) : InetAddress{} {
        SetPort(port);
        address.sin_addr.s_addr = INADDR_ANY;
    }

    std::string InetAddress::ToString() const {
        return fmt::format("{}:{}", Address(), Port());
    }

    std::string InetAddress::Address() const {
        char buffer[INET_ADDRSTRLEN];
        auto ret = ::inet_ntop(AF_INET, &address.sin_addr, buffer, INET_ADDRSTRLEN);
        return ret == nullptr ? "" : buffer;
    }

    bool InetAddress::SetAddress(const char *addr) {
        int ret = ::inet_pton(AF_INET, addr, &address.sin_addr);
        return ret == 0;
    }

    uint16_t InetAddress::Port() const {
        return ::ntohs(address.sin_port);
    }

    bool InetAddress::SetPort(uint16_t port) {
        address.sin_port = htons(port);
        return true;
    }
} // iphael