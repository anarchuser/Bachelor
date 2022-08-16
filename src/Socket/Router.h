#ifndef BACHELOR_ROUTER_H
#define BACHELOR_ROUTER_H

#include <chrono>
#include <thread>

#include "Socket.h"

#define LATENCY_US 200000

namespace bt {
    class Router : public Socket {
    public:
        explicit Router (port_t port, int timeout_ms = -1);

    private:
        void process (Packet const & packet, int sender) override;
    };
}


#endif //BACHELOR_ROUTER_H

/* Copyright © 2022 Aaron Alef */
