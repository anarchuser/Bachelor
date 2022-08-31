#ifndef BACHELOR_ROUTER_H
#define BACHELOR_ROUTER_H

#include <chrono>
#include <glog/logging.h>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "log.h"
#include "config.h"
#include "Packet/port.h"
#include "Packet/Packet.h"
#include "Chrono/Checkpoint.h"
#include "Packet/port.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

#define ROUTER_TIMEOUT_MS 8000

namespace bt {
    class Router final {
    public:
        port_t const port;

        explicit Router (port_t port, timestamp_t timeout_ms = ROUTER_TIMEOUT_MS);
        Router (Router const &) = delete;
        Router (Router &&) = delete;
        ~Router();

        void await_idle (timestamp_t idle) const;

    private:
        void service ();
        void send (Packet const & packet, in_addr_t receiver_address) const;

        struct sockaddr_in const address;
        int const socket_fd;

        std::thread thread;
        std::atomic <bool> should_stop = false;
        Checkpoint checkpoint;
        std::chrono::milliseconds const timeout;
    };
}


#endif //BACHELOR_ROUTER_H

/* Copyright © 2022 Aaron Alef */
