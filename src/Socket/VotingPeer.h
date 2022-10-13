#ifndef BACHELOR_VOTINGPEER_H
#define BACHELOR_VOTINGPEER_H

#include <unordered_map>
#include <memory>

#include "State/Action.h"
#include "State/Ballot.h"

#include "Peer.h"

namespace bt {

    class VotingPeer : public Peer {
    public:
        explicit VotingPeer (port_t port, int state, int timeout_ms = PEER_TIMEOUT_MS);

        timestamp_t act (ActionType what) override;
        timestamp_t act (state_t value) override;
        timestamp_t move (Position move) override;
        void process (VotePacket const & packet) override;

    private:
        mutable std::mutex mx;

        std::unordered_map <timestamp_t, Ballot> pending_actions;
    };

} // bt

#endif //BACHELOR_VOTINGPEER_H

/* Copyright © 2022 Aaron Alef */
