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

//        std::lock_guard <std::mutex> guard (mx);
        if (joiner == port || sender == port) return;
        if (joiner == sender) {
            connect (joiner);
            for (auto const & peer : peers) {
                tell (peer.first, joiner);
                tell (joiner, peer.first);
            }
        } else if (peers.contains (sender) || !peers [sender].contains (joiner)) {
            connect (joiner);
            tell (sender, joiner);
            peers [sender].insert (joiner);
        } else {
            LOG (WARNING) << PRINT_PORT << "Received connect request from unknown peer " << sender;
            LOG (WARNING) << PRINT_PORT << packet;
        }
    }

    bool Peer::connect (port_t peer) {
        if (peers.contains (peer)) return false;
        LOG (INFO) << PRINT_PORT << "[JOIN|" << peer << "]";

        send ({peer, port, ConnectPayload (port).c_str()}, router ?: peer);
        peers.insert ({peer, {}});
        return true;
    }

    void Peer::tell (port_t whom, port_t about) {
        if (whom == about) return;
        if (peers [whom].contains (about)) return;
        LOG (INFO) << PRINT_PORT << "[TELL|" << whom << "|" << about << "]";
        send ({whom, port, ConnectPayload (about).c_str()}, router ?: whom);
    }

    std::ostream & Peer::operator << (std::ostream & os) const {
        os << PRINT_PORT << "[LIST|peers|contacts]";
        for (auto const & requester : peers) {
            os << "\n\t" << requester.first << ": [";
            for (auto pending : requester.second) {
                os << pending << "|";
            }
            os << "]";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
