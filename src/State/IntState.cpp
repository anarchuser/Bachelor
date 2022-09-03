#include "IntState.h"

namespace bt {
    IntState::IntState (IntState const & other)
            : State (other)
            , state {other.state}
            {}

    timestamp_t IntState::apply (Action action) {
        std::lock_guard guard (mx);
        auto now = get_timestamp();
        // TODO: make a function "is_allowed"
        LOG_IF (WARNING, action.what == FORBIDDEN) << "\tRequested prohibited action!";
        actions.insert (action);
        return now;
    }

    state_t IntState::getState() const {
        return std::accumulate (actions.begin(), actions.end(), state);
    }

    std::ostream & operator << (std::ostream & os, IntState const & state) {
        os << "\tState: " << state.getState();
        if (kLogState) {
            os << "\n";
            for (auto action: state.getActions ()) {
                os << "\t" << action << "\n";
            }
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
