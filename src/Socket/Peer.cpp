#include "Peer.h"

namespace bt {
    Peer::Peer (bt::port_t port, int timeout_ms)
            : Socket (port, timeout_ms)
            {}

    Peer::~Peer () noexcept {
        Socket::~Socket();
        while (!is_destroyed_view) std::this_thread::yield();
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
        auto const & payload = * (Payload const *) packet.payload;
        switch (payload.action) {
            case PING:
                process_ping (packet);
                break;
            case CONNECT:
                process_connect (packet);
                break;
            case ACK:
            case NACK:
            default:
                LOG (WARNING) << PRINT_PORT << "Cannot handle action " << payload.action;
        }
    }

    void Peer::process_ping (Packet const & packet) {

    }

    void Peer::process_connect (Packet const & packet) {
        auto joiner = ((ConnectPayload const *) packet.payload)->joiner;
        auto sender = packet.header.sender;

        if (joiner == port || sender == port) return;
        if (consistent_peers.contains (joiner)) return;
        if (joiner == sender) {
            if (requested_peers.contains (joiner)) return;

            for (auto peer : consistent_peers) {
                connect (joiner, peer);
            }
            connect (sender);
        } else if (consistent_peers.contains (sender)) {
            // sender tells me about joiner:
            connect (joiner);
            LOG_ASSERT (requested_peers.contains (joiner));
            requested_peers [joiner].erase (sender);
            if (requested_peers [joiner].empty()) {
                consistent_peers.insert (joiner);
                requested_peers.erase (joiner);
            }

            for (auto peer : consistent_peers) {
                connect (joiner, peer);
            }
        } else {
            LOG (WARNING) << PRINT_PORT << "Received connect request from unknown peer " << sender;
            LOG (WARNING) << PRINT_PORT << packet;
        }
    }

    void Peer::connect (port_t peer) {
        send ({peer, port, ConnectPayload (port).c_str()}, router ?: peer);
        if (consistent_peers.empty()) {
            consistent_peers.insert (peer);
        } else {
            if (! requested_peers.contains (peer)) {
                requested_peers.insert ({peer, consistent_peers});
            }
        }
    }

    void Peer::connect (port_t a, port_t b) {
        send ({a, port, ConnectPayload (b).c_str()}, router ?: a);
        send ({b, port, ConnectPayload (a).c_str()}, router ?: b);
    }

    std::ostream & Peer::operator << (std::ostream & os) const {
        os << PRINT_PORT << "!";
        for (auto peer : consistent_peers) {
            os << peer << "|";
        }
        for (auto const & requester : requested_peers) {
            os << "\n\t" << requester.first << ": ?";
            for (auto pending : requester.second) {
                os << pending << "|";
            }
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
