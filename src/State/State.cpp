#include "State.h"

namespace bt {
    timestamp_t State::apply (Action action) {
        timestamp_t now = get_timestamp();
        actions.emplace_back (action, now);
        return now;
    }

    std::vector <std::pair <Action, timestamp_t>> const & State::getActions() const {
        return actions;
    }

    std::ostream & operator << (std::ostream & os, State const & state) {
        if (state.getActions().empty()) {
            return os << "[UNINITIALISED]\n";
        }
        for (auto const & [action, timestamp] : state.getActions()) {
            os << "\t" << action.port;
            os << " @" << timestamp;
            os << ": " << action.type;
            os << " @" << action.timestamp;
            os << "\n";
        }
    }
}

/* Copyright © 2022 Aaron Alef */
