#ifndef BACHELOR_ROUTER_H
#define BACHELOR_ROUTER_H

#include <queue>
#include <ctime>
#include <chrono>
#include <glog/logging.h>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <iostream>

#include "log.h"
#include "config.h"
#include "Packet/port.h"
#include "Packet/Packet.h"
#include "Packet/port.h"
#include "Packet/helper.h"
#include "Chrono/Checkpoint.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

#define ROUTER_TIMEOUT_MS 20000

#define ROUTER_LATENCY 100

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
        void process ();
        void send (Packet const & packet, in_addr_t receiver_address) const;

        struct sockaddr_in const address_in, address_out;
        int const recv_fd;
        int const send_fd;

        std::thread receiver, sender;
        std::atomic <bool> should_stop = false;
        std::atomic <bool> receive_stop = false;
        Checkpoint checkpoint;
        std::chrono::milliseconds const timeout;

        std::mutex mutable mx;
        std::atomic <bool> queue_empty = true;
        std::queue <std::tuple <std::chrono::steady_clock::time_point, std::string, in_addr_t>> queue;
    };
}


#endif //BACHELOR_ROUTER_H

/* Copyright © 2022 Aaron Alef */
