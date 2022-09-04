#include "State.h"

namespace bt {
    State::State (State const & other): actions {other.getActions()} {}

    timestamp_t State::apply (Action action) {
        std::lock_guard guard (mx);
        timestamp_t now = get_timestamp();
        actions.insert (action);
        return now;
    }

    std::set <Action> State::getActions() {
        std::set <Action> copy;
        for (auto action : actions) {
            copy.insert (action);
        }
        return copy;
    }
    std::set <Action> State::getActions() const {
        std::lock_guard guard (mx);
        std::set <Action> copy;
        for (auto action : actions) {
            copy.insert (action);
        }
        return copy;
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
