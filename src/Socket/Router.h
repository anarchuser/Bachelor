#ifndef BACHELOR_ROUTER_H
#define BACHELOR_ROUTER_H

#include <chrono>
#include <thread>

#include "Socket.h"

#define LATENCY_US 200000
#define ROUTER_TIMEOUT_MS -1

namespace bt {
    class Router : public Socket {
    public:
        explicit Router (port_t port, int timeout_ms = ROUTER_TIMEOUT_MS);
        Router (Router const &) = delete;
        ~Router() override;

    private:
        void process (Packet const & packet, port_t sender) override;
    };
}


#endif //BACHELOR_ROUTER_H

/* Copyright © 2022 Aaron Alef */
