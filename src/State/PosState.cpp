#include "PosState.h"

namespace bt {
    bool PosState::try_apply (Action action, Position state, std::set<Action> actions) {
        switch (action.what) {
            case NOOP:
                return true;
            case FORBIDDEN:
                return false;
            case ADD:
            case MOVE:
            default:;
        }
        if (actions.contains (action)) return * actions.find (action) == action;

        auto [action_pos, _] = actions.insert (action);
        for (auto trav = actions.begin(); trav != action_pos; trav++) {
            state += trav->value.move.delta;
            LOG_IF (WARNING, state != trav->value.move.reference) << "Given actions cannot be applied iteratively onto the given initialState!";
        }
        for (auto trav = action_pos; trav != actions.end(); trav++) {
            state += trav->value.move.delta;
            if (state != trav->value.move.reference) return false;
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

    Position PosState::getState() const {
        return std::accumulate (actions.begin(), actions.end(), initialState);
    }

    std::ostream & operator << (std::ostream & os, PosState const & state) {
        os << state.getState();
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
