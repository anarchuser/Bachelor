#ifndef BACHELOR_VOTINGPEER_H
#define BACHELOR_VOTINGPEER_H

#include "Peer.h"

namespace bt {

    class VotingPeer : public Peer {
    public:
        explicit VotingPeer (port_t port, int state, int timeout_ms = PEER_TIMEOUT_MS);

        timestamp_t act (ActionType what) override;
        timestamp_t act (state_t value) override;
        void process (VotePacket const & packet) override;
    };

} // bt

#endif //BACHELOR_VOTINGPEER_H

/* Copyright © 2022 Aaron Alef */
