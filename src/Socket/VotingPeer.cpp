#include "VotingPeer.h"

namespace bt {
    VotingPeer::VotingPeer (bt::port_t port, int state, int timeout_ms): Peer (port, state, timeout_ms) {}

    timestamp_t VotingPeer::act (ActionType what) {
        Action action (port, what);
        vote (action, APPROVE);
        // TODO: add to temporary state
        return consistent_state.apply (action);
    }
    timestamp_t VotingPeer::act (state_t value) {
        Action action (port, value);
        vote (action, APPROVE);
        // TODO: add to temporary state
        consistent_state.apply (action);
        return action.when;
    }
}

/* Copyright © 2022 Aaron Alef */
