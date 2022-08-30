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
#include "State/State.h"

#define PEER_TIMEOUT_MS 5000

namespace bt {
    class Peer: public Socket {
    public:
        explicit Peer (port_t port, int timeout_ms = PEER_TIMEOUT_MS);
        Peer (Peer const &) = delete;
        ~Peer() noexcept override;

        void connect (port_t peer);

        [[nodiscard]] std::set <port_t> const & getPeers() const;

        std::atomic <std::size_t> num_of_peers = 0;

        [[nodiscard]] State getState() const;

    private:
        std::set <port_t> peers;
        std::atomic <std::uint32_t> message_counter = 0;

        State consistent_state;

        void tell (port_t whom, port_t about);
        void process (Packet const & packet, port_t sender) override;
        void process (PingPacket const & packet);
        void process (ConnectPacket const & packet);
        void process (ActionPacket const & packet);
    };

    std::ostream & operator << (std::ostream & os, Peer const & peer);
}


#endif //BACHELOR_PEER_H

/* Copyright © 2022 Aaron Alef */
