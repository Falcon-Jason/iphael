/**
  * @file InetAddress.h
  * @author jason
  * @date 2022/2/19
  */

#pragma once
#include <arpa/inet.h>
#include <string>

namespace iphael {
    /**
     * @class InetAddress
     * Data abstraction class of internet addresses(encapsulated native `sockaddr_in`).
     */
    class InetAddress {
    private:
        sockaddr_in address;

    public:
        /**
         * Create an InetAddress of "0.0.0.0:0"
         */
        InetAddress();

        /**
         * Create an InetAddress of "0.0.0.0:@param port
         */
        explicit InetAddress(uint16_t port);

        /**
         * Create an InetAddress of given address and port.
         * Create an InetAddress of "0.0.0.0:0" if the given address is invalid,
         * @param addr The given address, should be in form of "1.2.3.4".
         * @param port The given port.
         */
        InetAddress(const char *addr, uint16_t port);

        InetAddress(const InetAddress &rhs) = default;

        ~InetAddress() = default;

        /**
         * @return The IP address of `this`, in form of "1.2.3.4"
         */
        [[nodiscard]] std::string Address() const;

        /**
         * @return The port of `this`.
         */
        [[nodiscard]] uint16_t Port() const;

        /**
         * @return The string presentation of `this`, in form of "Address:Port"
         */
        [[nodiscard]] std::string ToString() const;

        /**
         * Set the address of `this`.
         * Do nothing if the `addr` is invalid.
         * @param addr The given new address, in form of "1.2.3.4"
         * @return Whether the given `addr` is valid.
         */
        bool SetAddress(const char *addr);

        /**
         * Set the port of `this`.
         * @param port The given new port.
         * @retrun Whether the given `port` is valid (It's always true now).
         */
        bool SetPort(uint16_t port);

        [[nodiscard]] constexpr auto Size() const {
            return sizeof(address);
        }

        [[nodiscard]] inline sockaddr *SockAddr() {
            return reinterpret_cast<sockaddr *>(&address);
        }

        [[nodiscard]] inline const sockaddr *SockAddr() const {
            return reinterpret_cast<const sockaddr *>(&address);
        }
    };
} // iphael
