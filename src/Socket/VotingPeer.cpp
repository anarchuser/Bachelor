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

    void VotingPeer::process (VotePacket const & packet) {
        LOG_IF (INFO, kLogRecvVote) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        // TODO: make this smarter
        if (rejected_actions.contains (packet.action.when)) return;

        if (!consistent_state.contains (packet.action)) {
            // TODO: apply actions temporarily only
            bool shouldReject = !consistent_state.apply (packet.action);
            if (shouldReject) rejected_actions.insert (packet.action.when);
            vote (packet.action, shouldReject ? REJECT : APPROVE);
        } else {
            // TODO: Update vote count on temporary action
        }
    }
}

/* Copyright © 2022 Aaron Alef */
