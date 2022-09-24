#include "NaivePeer.h"

namespace bt {
    NaivePeer::NaivePeer (bt::port_t port, int state, int timeout_ms): Peer (port, state, timeout_ms) {}

    timestamp_t NaivePeer::act (ActionType what) {
        Action action (port, what);
        vote (action, APPROVE);
        return consistent_state.apply (action);
    }
    timestamp_t NaivePeer::act (state_t value) {
        Action action (port, value);
        vote (action, APPROVE);
        consistent_state.apply (action);
        return action.when;
    }
}

/* Copyright © 2022 Aaron Alef */
