#include "Router.h"

namespace bt {
    Router::Router (port_t port): Socket (port) {}

    void Router::process (Packet const & packet, int sender) {
        LOG (INFO) << '\t' << port << ": Sending package from " << sender << " to " << packet.header.receiver;

        Socket::process (packet, sender);

        std::this_thread::sleep_for (std::chrono::microseconds (LATENCY_US));

        send (packet, packet.header.receiver);
    }
}

/* Copyright © 2022 Aaron Alef */
