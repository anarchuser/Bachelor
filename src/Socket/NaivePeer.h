#ifndef BACHELOR_NAIVEPEER_H
#define BACHELOR_NAIVEPEER_H

#include "Peer.h"

namespace bt {

    class NaivePeer : public Peer {
    public:
        explicit NaivePeer (port_t port, int state, int timeout_ms = PEER_TIMEOUT_MS);

        timestamp_t act (ActionType what) override;
        timestamp_t act (state_t value) override;
    };

} // bt

#endif //BACHELOR_NAIVEPEER_H

/* Copyright © 2022 Aaron Alef */
