#include "Router.h"

namespace bt {
    Router::Router (port_t port, int timeout): Socket (port, timeout) {}

    void Router::process (Packet const & packet, int sender) {
        Socket::process (packet, sender);

        LOG (INFO) << '\t' << port << ": [ROUT|" << sender << " -> " << packet.header.receiver << "]";

//        std::this_thread::sleep_for (std::chrono::microseconds (LATENCY_US));

        send (packet, packet.header.receiver);
    }
}

/* Copyright © 2022 Aaron Alef */
