#ifndef BACHELOR_PEER_H
#define BACHELOR_PEER_H

#include <chrono>
#include <map>
#include <set>
#include <unordered_set>
#include <thread>
#include <ostream>

#include "config.h"
#include "Socket.h"
#include "log.h"
#include "Packet/helper.h"
#include "Packet/Packet.h"
#include "Packet/ConnectPacket.h"
#include "Packet/PingPacket.h"
#include "Packet/VotePacket.h"
#include "State/IntState.h"

#define PEER_TIMEOUT_MS 5000

namespace bt {
    class Peer: public Socket {
    public:
        explicit Peer (port_t port, int state, int timeout_ms = PEER_TIMEOUT_MS);
        Peer (Peer const &) = delete;
        ~Peer() noexcept override = 0;

        void connect (port_t peer);
        virtual timestamp_t act (ActionType what) = 0;
        virtual timestamp_t act (state_t value) = 0;

        [[nodiscard]] inline std::set <port_t> const & getPeers() const { return peers; }
        [[nodiscard]] inline IntState getState() const { return consistent_state; }

        std::atomic <std::size_t> num_of_peers = 0;

    protected:
        IntState consistent_state;
        std::unordered_set <timestamp_t> rejected_actions;

        virtual void process (VotePacket const & packet) = 0;
        virtual void vote (Action action, Vote vote) final;

    private:
        void introduce (port_t whom);
        void introduce (port_t new_peer, port_t old_peer);
        void process (Packet const & packet, port_t sender) override;
        void process (PingPacket const & packet);
        void process (ConnectPacket const & packet);

        std::set <port_t> peers;
        std::atomic <std::uint32_t> msg_counter = 0;
        [[nodiscard]] inline std::uint32_t count_msg () { return msg_counter++; }
    };

    std::ostream & operator << (std::ostream & os, Peer const & peer);
}


#endif //BACHELOR_PEER_H

/* Copyright © 2022 Aaron Alef */
