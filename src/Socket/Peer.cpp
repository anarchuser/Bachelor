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
                process_ping (packet, sender);
                break;
            case CONNECT:
                process_connect (packet, sender);
                break;
            case ACK:
            case NACK:
            default:
                LOG (WARNING) << PRINT_PORT << "Cannot handle action " << payload.action;
        }
    }

    void Peer::process_ping (Packet const & packet, port_t sender) {

    }

    void Peer::process_connect (Packet const & packet, port_t sender) {
        // TODO: only apply joins permanently if all other known peers agree

        auto joiner = ((ConnectPayload const *) packet.payload)->joiner;
        if (peers.contains (joiner)) return;

        if (packet.header.sender == joiner) {
            LOG (INFO) << PRINT_PORT << joiner << " joined the network";
            for (auto peer : peers) {
                send ({peer, port, ConnectPayload (joiner).c_str()});
            }
            peers.insert (joiner);
        }
        else if (peers.contains (packet.header.sender)) {
            LOG (INFO) << PRINT_PORT << packet.header.sender << " let me know of " << joiner;
            join (joiner);
        }
        else {
            LOG (WARNING) << PRINT_PORT << "Unknown peer " << packet.header.sender << " told me of " << joiner;
        }
    }

    void Peer::join (port_t peer) {
        send ({peer, port, ConnectPayload (port).c_str()}, router ?: peer);
        peers.insert (peer);
    }

    std::ostream & Peer::operator << (std::ostream & os) const {
        os << PRINT_PORT << "|";
        for (auto peer : peers) {
            os << peer << "|";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
