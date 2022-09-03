#include "Peer.h"

namespace bt {
    Peer::Peer (bt::port_t port, int state, int timeout_ms)
            : Socket (port, timeout_ms)
            , consistent_state {state}
            {}

    Peer::~Peer () noexcept {
        Socket::~Socket();
        while (!is_destroyed_view) std::this_thread::yield();

        if (kLogPeerDtorState) std::cout << * this << std::endl;
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
        LOG_IF (INFO, kLogRecv) << PRINT_PORT << "[RECV|" << sender << "]\t[" << to_string (packet) << "]";

        DISPATCH (process, packet);
    }

    void Peer::process (PingPacket const & packet) {
        LOG_IF (INFO, kLogRecvPing) << PRINT_PORT << "[RECV]\t[" << packet << "]";
    }

    void Peer::process (ConnectPacket const & packet) {
        auto sender = packet.sender;
        auto joiner = packet.joiner;

        if (peers.contains (joiner)) return;
        if (joiner == port || sender == port) return;

        LOG_IF (INFO, kLogRecvConnect) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        introduce (joiner);
        connect (joiner);
    }

    void Peer::process (ActionPacket const & packet) {
        LOG_IF (INFO, kLogRecvAction) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        // TODO: broadcast an ACK

        consistent_state.apply (packet.action);
    }

    void Peer::connect (port_t peer) {
        if (peers.contains (peer)) return;

        LOG_IF (INFO, kLogJoin) << PRINT_PORT << "[JOIN|" << peer << "]";
        ConnectPacket msg (peer, port, port, count_msg());
        send (msg, router_port.load() ?: peer);
        peers.insert (peer);
        ++num_of_peers;
    }

    timestamp_t Peer::act (ActionType what) {
        Action action (port, what);
        consistent_state.apply (action);
        for (auto peer : peers) {
            send (ActionPacket (peer, port, action, count_msg()));
        }
        return action.when;
    }
    timestamp_t Peer::act (state_t value) {
        Action action (port, value);
        consistent_state.apply (action);
        for (auto peer : peers) {
            send (ActionPacket (peer, port, action, count_msg()));
        }
        return action.when;
    }

    void Peer::introduce (port_t new_peer, port_t old_peer) {
        if (new_peer == old_peer) return;

        LOG_IF (INFO, kLogTell) << PRINT_PORT << "[TELL|" << old_peer << "|" << new_peer << "]";
        ConnectPacket msg (old_peer, port, new_peer, count_msg ());
        send (msg, router_port.load() ?: new_peer);
    }
    void Peer::introduce (port_t whom) {
        for (auto peer : peers) {
            introduce (peer, whom);
            introduce (whom, peer);
        }
    }

    std::ostream & operator << (std::ostream & os, Peer const & peer) {
        os << peer.port << ": [PEER|";
        os << "Σ" << peer.num_of_peers;
        for (auto neighbour : peer.getPeers()) os << "|" << neighbour;
        os << "]\n";
        os << peer.getState();
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
