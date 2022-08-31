#include "State.h"

namespace bt {
    timestamp_t State::apply (Action action) {
        timestamp_t now = get_timestamp();
        actions.emplace_back (action);
        return now;
    }

    std::vector <Action> const & State::getActions() const {
        return actions;
    }

    std::ostream & operator << (std::ostream & os, State const & state) {
        if (state.getActions().empty()) {
            return os << "[UNINITIALISED]\n";
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
