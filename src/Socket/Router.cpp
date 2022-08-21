#include "Router.h"

namespace bt {
    Router::Router (port_t port, int timeout)
            : Socket (port, timeout)
            {}

    Router::~Router () {
        Socket::~Socket();
        while (!is_destroyed_view) std::this_thread::yield();
    }

    void Router::process (Packet const & packet, port_t sender) {
        Socket::process (packet, sender);
        LOG (INFO) << PRINT_PORT << "[ROUT|" << sender << " -> " << packet.header.receiver << "]";

        // TODO: make this non-blocking
        std::this_thread::sleep_for (std::chrono::microseconds (LATENCY_US));
        send (packet, packet.header.receiver);
    }
}

/* Copyright © 2022 Aaron Alef */
