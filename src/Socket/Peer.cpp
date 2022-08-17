#include "Peer.h"

namespace bt {
    Peer::Peer (bt::port_t port, int timeout_ms)
            : Socket (port, timeout_ms)
            {}

    Peer::~Peer () noexcept {
        Socket::~Socket();
        while (!is_destroyed) std::this_thread::yield();
    }

    void Peer::process (Packet const & packet, port_t sender) {
        Socket::process (packet, sender);

        if (packet.header.sender < PORT_PEER_START || packet.header.sender > PORT_PEER_END) {
            LOG (WARNING) << PRINT_PORT << "Received packet from suspicious port - " << packet;
            return;
        }
        if (packet.header.receiver != port) {
            LOG (WARNING) << PRINT_PORT << "Received packet with foreign recipient - " << packet;
            return;
        }
        if (packet.header.action == CONNECT) {
            auto joiner = * (port_t *) packet.content;
            if (peers.contains (joiner)) return;

            if (packet.header.sender == joiner) {
                LOG (INFO) << PRINT_PORT << joiner << " joined the network";
                peers.insert (joiner);
                for (auto peer : peers) {
                    send ({peer, port, (char const *) & joiner});
                }
            }
            else if (peers.contains (packet.header.sender)) {
                LOG (INFO) << PRINT_PORT << packet.header.sender << " let me know of " << joiner;
                peers.insert (joiner);
            }
            else {
                LOG (WARNING) << PRINT_PORT << "Unknown peer " << packet.header.sender << " told me of " << joiner;
            }
        }
    }
}

/* Copyright © 2022 Aaron Alef */
