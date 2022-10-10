#include "PosState.h"

namespace bt {
    bool PosState::try_apply (Action action, state_t state, std::set<Action> actions) {
        if (action.what != ADD) return action.what == NOOP;
        if (actions.contains (action)) return * actions.find (action) == action;

        auto [action_pos, _] = actions.insert (action);
        for (auto trav = actions.begin(); trav != action_pos; trav++) {
//            state += trav->value;
            LOG_IF (WARNING, state < 0) << "Given actions cannot be applied iteratively onto the given initialState!";
        }
        for (auto trav = action_pos; trav != actions.end(); trav++) {
//            state += trav->value;
            if (state < 0) return false;
        }
        return true;
    }

    timestamp_t PosState::apply (Action action) {
        std::lock_guard guard (mx);
        auto canApply = try_apply (action, initialState, getActions());
        if (!canApply) {
            LOG_IF (WARNING, kLogInvalidAction) << "\tProhibited action requested: " << action;
            return 0;
        }

        auto now = get_timestamp();
        actions.insert (action);
        return now;
    }

    bool PosState::contains (Action action) const {
        std::lock_guard guard (mx);
        return actions.contains (action);
    }

    state_t PosState::getState() const {
        return std::accumulate (actions.begin(), actions.end(), initialState);
    }

    std::ostream & operator << (std::ostream & os, PosState const & state) {
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
