#ifndef BACHELOR_PEER_H
#define BACHELOR_PEER_H

#include <chrono>
#include <set>
#include <thread>

#include "config.h"
#include "Socket.h"

#define PEER_TIMEOUT_MS 5000

namespace bt {
    class Peer: public Socket {
    public:
        explicit Peer (port_t port, int timeout_ms = PEER_TIMEOUT_MS);
        Peer (Peer const &) = delete;
        ~Peer() noexcept override;

    private:
        std::set <port_t> peers;

        void process (Packet const & packet, port_t sender) override;
    };
}


#endif //BACHELOR_PEER_H

/* Copyright © 2022 Aaron Alef */
