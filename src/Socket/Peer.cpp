#include "Peer.h"

namespace bt {
    Peer::Peer (bt::port_t port, int timeout_ms)
            : Socket (port, timeout_ms)
            {}

    Peer::~Peer () noexcept {
        Socket::~Socket();
        while (!is_destroyed_view) std::this_thread::yield();
        this->operator << (std::cout) << std::endl;
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
        switch (payload.type) {
            case PING:
                process_ping (packet);
                break;
            case CONNECT:
                process_connect (packet);
                break;
            case ACK:
            case NACK:
            default:
                LOG (WARNING) << PRINT_PORT << "Cannot handle type " << payload.type;
        }
    }

    void Peer::process_ping (Packet const & packet) {

    }

    void Peer::process_connect (Packet const & packet) {
        auto joiner = ((ConnectPayload const *) packet.payload)->joiner;
        auto sender = packet.header.sender;

        if (peers.contains (joiner)) return;
        if (joiner == port || sender == port) return;

        for (auto peer : peers) {
            tell (peer, joiner);
            tell (joiner, peer);
        }
        connect (joiner);
    }

    void Peer::connect (port_t peer) {
        if (peers.contains (peer)) return;

        LOG (INFO) << PRINT_PORT << "[JOIN|" << peer << "]";
        send ({peer, port, ConnectPayload (port).c_str()}, router ?: peer);
        peers.insert (peer);
        ++num_of_peers;
    }

    void Peer::tell (port_t whom, port_t about) {
        if (whom == about) return;

//        LOG (INFO) << PRINT_PORT << "[TELL|" << whom << "|" << about << "]";
        send ({whom, port, ConnectPayload (about).c_str()}, router ?: whom);
    }

    std::set <port_t> const & Peer::get_peers() const {
        return peers;
    }

    std::ostream & Peer::operator << (std::ostream & os) const {
        os << PRINT_PORT << "[PEER|";
        os << "Σ" << num_of_peers;
        for (auto peer : peers) {
            os << "|" << peer;
        }
        return os << ']';
    }
}

/* Copyright © 2022 Aaron Alef */
