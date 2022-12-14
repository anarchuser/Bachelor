#include "IntState.h"

namespace bt {
    bool IntState::try_apply (Action action, state_t state, std::map <Action, timestamp_t> actions) {
        if (action.what != ADD) return action.what != FORBIDDEN;
        if (actions.contains (action)) return actions.find (action)->first == action;

        auto [action_pos, _] = actions.emplace (action, 0);
        for (auto trav = actions.begin(); trav != action_pos; trav++) {
            state += trav->first.value.change;
            LOG_IF (WARNING, state < 0) << "Given actions cannot be applied iteratively onto the given initialState!";
        }
        for (auto trav = action_pos; trav != actions.end(); trav++) {
            state += trav->first.value.change;
            if (state < 0) return false;
        }
        return true;
    }

    timestamp_t IntState::apply (Action action) {
        std::lock_guard guard (mx);
        auto canApply = try_apply (action, initialState, getActions());
        if (!canApply) {
            LOG_IF (WARNING, kLogInvalidAction) << "\tProhibited action requested: " << action;
            return 0;
        }

        auto now = get_timestamp();
        actions.emplace (action, now);
        return now;
    }

    bool IntState::contains (Action action) const {
        std::lock_guard guard (mx);
        return actions.contains (action);
    }

    state_t IntState::getState() const {
        return std::accumulate (actions.begin(), actions.end(), initialState);
    }

    std::ostream & operator << (std::ostream & os, IntState const & state) {
        os << "\tState: " << state.getState();
        if (kLogState) {
            os << "\n";
            for (auto action: state.getActions ()) {
                os << "\t" << action.first << "\n";
            }
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
