#ifndef BACHELOR_SOCKET_H
#define BACHELOR_SOCKET_H

#include <glog/logging.h>
#include <iostream>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <atomic>
#include <thread>

#include <netdb.h>

#include "log.h"
#include "Packet/helper.h"
#include "Packet/Packet.h"
#include "Chrono/Checkpoint.h"

#define SOCKET_TIMEOUT_MS 0

namespace bt {
    struct Socket {
    public:
        port_t const port;
        static std::atomic <in_addr_t> router_address;
        static std::atomic <port_t>    router_port;

        /* Bind a duplex socket to the port.
         * Checkpoint specifies how to behave on destruction:
         * Negative: Never time out, run forever
         * Zero:     Instantly time out
         * Positive: Stop if no message within this time arrived
         */
        explicit Socket (port_t port, timestamp_t timeout_ms = SOCKET_TIMEOUT_MS);
        Socket (Socket const &) = delete;
        virtual ~Socket();

        virtual void send (Packet const & packet, port_t receiver) final;
        virtual void send (Packet const & packet) final;

    protected:
        virtual void process (Packet const & packet, port_t sender);

        std::atomic <bool> const & is_destroyed_view = is_destroyed;

    private:
        struct sockaddr_in const address;
        int const socket_fd;

        std::thread thread;
        std::atomic <bool> should_stop = false;
        std::atomic <bool> is_destroyed = false;
        Checkpoint checkpoint;
        std::chrono::milliseconds const timeout;

        void service ();
    };
}

#endif //BACHELOR_SOCKET_H

/* Copyright © 2022 Aaron Alef */
