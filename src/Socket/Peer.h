#ifndef BACHELOR_PEER_H
#define BACHELOR_PEER_H

#include <chrono>
#include <set>
#include <unordered_map>
#include <thread>
#include <ostream>

#include "config.h"
#include "Socket.h"

#include "Packet/Payload.h"

#define PEER_TIMEOUT_MS 5000

namespace bt {
    class Peer: public Socket {
    public:
        explicit Peer (port_t port, int timeout_ms = PEER_TIMEOUT_MS);
        Peer (Peer const &) = delete;
        ~Peer() noexcept override;

        void connect (port_t peer);

        std::ostream & operator << (std::ostream & os) const;

    private:
        std::unordered_map <port_t, std::set <port_t>> requested_peers;
        std::set <port_t> consistent_peers;

        void connect (port_t a, port_t b);
        void process (Packet const & packet, port_t sender) override;
        void process_ping (Packet const & packet);
        void process_connect (Packet const & packet);
    };
}


#endif //BACHELOR_PEER_H

/* Copyright © 2022 Aaron Alef */
