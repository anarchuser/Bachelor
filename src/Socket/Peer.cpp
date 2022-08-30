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
        if (packet.sender < PORT_PEER_START || packet.sender > PORT_PEER_END) {
            LOG (WARNING) << PRINT_PORT << "Received packet from suspicious port - " << packet;
            return;
        }
        if (packet.receiver != port) {
            LOG (WARNING) << PRINT_PORT << "Received packet with foreign recipient - " << packet;
            return;
        }
        DISPATCH (process, packet);
    }

    void Peer::process (PingPacket const & packet) {
        LOG_IF (INFO, kLogRecv) << PRINT_PORT << "[RECV]\t[" << packet << "]";
    }

    void Peer::process (ConnectPacket const & packet) {
        auto sender = packet.sender;
        auto joiner = packet.joiner;

        if (peers.contains (joiner)) return;
        if (joiner == port || sender == port) return;

        LOG_IF (INFO, kLogRecv) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        for (auto peer : peers) {
            tell (peer, joiner);
            tell (joiner, peer);
        }
        connect (joiner);
    }

    void Peer::process (ActionPacket const & packet) {

    }

    void Peer::connect (port_t peer) {
        if (peers.contains (peer)) return;

        LOG_IF (INFO, kLogJoin) << PRINT_PORT << "[JOIN|" << peer << "]";
        ConnectPacket msg (peer, port, port, message_counter++);
        send (msg, router_port.load() ?: peer);
        peers.insert (peer);
        ++num_of_peers;
    }

    void Peer::tell (port_t whom, port_t about) {
        if (whom == about) return;

        LOG_IF (INFO, kLogTell) << PRINT_PORT << "[TELL|" << whom << "|" << about << "]";
        ConnectPacket msg (whom, port, about, message_counter++);
        send (msg, router_port.load() ?: whom);
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
