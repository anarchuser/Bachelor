#ifndef BACHELOR_PEER_H
#define BACHELOR_PEER_H

#include <chrono>
#include <map>
#include <set>
#include <thread>
#include <ostream>

#include "config.h"
#include "Socket.h"
#include "log.h"
#include "Packet/helper.h"
#include "Packet/Packet.h"
#include "Packet/ActionPacket.h"
#include "Packet/ConnectPacket.h"
#include "Packet/PingPacket.h"
#include "State/IntState.h"

#define PEER_TIMEOUT_MS 5000

namespace bt {
    class Peer: public Socket {
    public:
        explicit Peer (port_t port, int state, int timeout_ms = PEER_TIMEOUT_MS);
        Peer (Peer const &) = delete;
        ~Peer() noexcept override;

        void connect (port_t peer);
        timestamp_t act (ActionType what);

        [[nodiscard]] inline std::set <port_t> const & getPeers() const { return peers; }
        [[nodiscard]] inline IntState getState() const { return consistent_state; }

        std::atomic <std::size_t> num_of_peers = 0;

    private:
        std::set <port_t> peers;
        IntState consistent_state;

        void introduce (port_t whom);
        void introduce (port_t new_peer, port_t old_peer);
        void process (Packet const & packet, port_t sender) override;
        void process (PingPacket const & packet);
        void process (ConnectPacket const & packet);
        void process (ActionPacket const & packet);

        std::atomic <std::uint32_t> msg_counter = 0;
        [[nodiscard]] inline std::uint32_t count_msg () { return msg_counter++; }
    };

    std::ostream & operator << (std::ostream & os, Peer const & peer);
}


#endif //BACHELOR_PEER_H

/* Copyright © 2022 Aaron Alef */
