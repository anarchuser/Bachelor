#include "State.h"

namespace bt {
    State::State (State const & other) {
        std::lock_guard other_lock (other.mx);
        std::lock_guard own_lock (mx);
        for (auto action : other.actions) {
            actions.insert (action);
        }
    }

    timestamp_t State::apply (Action action) {
        std::lock_guard guard (mx);
        timestamp_t now = get_timestamp();
        actions.insert (action);
        return now;
    }

    std::set <Action> const & State::getActions() const {
        std::lock_guard guard (mx);
        return actions;
    }

    std::ostream & operator << (std::ostream & os, State const & state) {
        if (state.getActions().empty()) {
            return os << "\t[UNINITIALISED]\n";
        }
        for (auto action : state.getActions()) {
            os << "\t" << action.who;
//            os << " @" << timestamp;
            os << ": " << action.what;
            os << " @" << action.when;
            os << "\n";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
