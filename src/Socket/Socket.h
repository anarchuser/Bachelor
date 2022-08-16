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

#include "Packet/Packet.h"
#include "Chrono/Timeout.h"

namespace bt {
    struct Socket {
    public:
        port_t const port;

        /* Bind a duplex socket to the port.
         * Timeout specifies how to behave on destruction:
         * Negative: Never time out, run forever
         * Zero:     Instantly time out
         * Positive: Stop if no message within this time arrived
         */
        explicit Socket (port_t port, int timeout_ms = 0);
        virtual ~Socket();

        virtual void service () final;

        virtual void send (Packet const & packet, port_t receiver);
        virtual void send (Packet const & packet);

    protected:
        virtual void process (Packet const & packet, port_t sender);
        std::atomic <bool> is_destroyed = false;

    private:
        struct sockaddr_in address = {0};
        int const socket_fd;

        std::thread thread;
        std::atomic <bool> should_stop = false;
        Timeout timeout;
    };
}

#endif //BACHELOR_SOCKET_H

/* Copyright © 2022 Aaron Alef */
