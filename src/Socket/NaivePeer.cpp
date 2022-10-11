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

    timestamp_t NaivePeer::move (PosChange move) {
        Action action (port, move);
        vote (action, APPROVE);
        positions.at (port).apply (action);
        return action.when;
    }

    void NaivePeer::process (VotePacket const & packet) {
        LOG_IF (INFO, kLogRecvVote) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        auto const & action = packet.action;

        if (rejected_actions.contains (action.when)) return;

        if (action.what == MOVE) {
            if (positions.at (action.who).contains (action)) return;

            bool shouldReject = !positions.at (action.who).apply (action);
            if (shouldReject) rejected_actions.insert (action.when);
        } else {
            if (consistent_state.contains (action)) return;

            bool shouldReject = !consistent_state.apply (action);
            if (shouldReject) rejected_actions.insert (action.when);
        }
    }
}

/* Copyright © 2022 Aaron Alef */
