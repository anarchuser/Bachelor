#include "VotingPeer.h"

namespace bt {
    VotingPeer::VotingPeer (bt::port_t port, int state, int timeout_ms)
            : Peer (port, state, timeout_ms) {}

    timestamp_t VotingPeer::act (ActionType what) {
        Action action (port, what);
        pending_actions.emplace (action.when, action);
        vote (action, APPROVE);
        return action.when;
    }

    timestamp_t VotingPeer::act (state_t value) {
        Action action (port, value);
        pending_actions.emplace (action.when, action);
        vote (action, APPROVE);
        return action.when;
    }

    timestamp_t VotingPeer::move (PosChange move) {
        Action action (port, move);
        pending_actions.emplace (action.when, action);
        vote (action, APPROVE);
        return action.when;
    }

    void VotingPeer::process (VotePacket const & packet) {
        LOG_IF (INFO, kLogRecvVote) << PRINT_PORT << "[RECV]\t[" << packet << "]";

        auto const & action = packet.action;
        if (rejected_actions.contains (action)) return;

        State * state = action.what == MOVE
                ? dynamic_cast <State *> (& positions.at (action.who))
                : dynamic_cast <State *> (& consistent_state);
        if (state->contains (action)) return;

        if (!pending_actions.contains (action.when)) {
            pending_actions.emplace (action.when, action);

            // TODO: determine whether to approve or reject
            // TODO: let states apply wrong actions
            auto ownVote = (Vote) (action.what != FORBIDDEN);

            vote (action, ownVote);
        } else {
            LOG_IF (WARNING, pending_actions.at (action.when).action != action)
                    << PRINT_PORT
                    << "Conflicting actions at time " <<  action.when
                    << ". Actions are:"
                    << "\nAlready pending: " << pending_actions.at (action.when).action
                    << "\nNewly requested: " << action;
        }

        auto & ballot = pending_actions.at (action.when);
        bool success = packet.vote
                ? ballot.approvedBy (packet.sender)
                : ballot.rejectedBy (packet.sender);
        LOG_IF (WARNING, !success) << PRINT_PORT << "Conflicting votes received! - " << packet;

        static auto majority = (num_of_peers + 1) / 2;
        LOG_ASSERT (ballot.approvers.size() <= majority || ballot.rejecters.size() <= majority);
        if (ballot.approvers.size() > majority) {
            auto timestamp = state->apply (action);

            if (timestamp) {
                LOG_IF (INFO, kLogConsistent) << PRINT_PORT << "Consistently applying action @" << timestamp << ": " << action;
                pending_actions.erase (action.when);
            }
        } else if (ballot.rejecters.size() > majority) {
            pending_actions.erase (action.when);
            rejected_actions.insert (action);
            LOG_IF (INFO, kLogConsistent) << PRINT_PORT << "Rejecting action " << action;
        }
    }
}

/* Copyright © 2022 Aaron Alef */
