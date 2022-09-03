#include "IntState.h"

namespace bt {
    IntState::IntState (IntState const & other) {
        std::lock_guard other_lock (other.mx);
        std::lock_guard own_lock (mx);
        state = other.state.load();
        for (auto action : other.actions) {
            actions.insert (action);
        }
    }

    timestamp_t IntState::apply (Action action) {
        std::lock_guard guard (mx);
        auto now = get_timestamp();
        switch (action.what) {
            case NOOP:
                break;
            case FORBIDDEN:
                LOG (WARNING) << "\tRequested prohibited action!";
                return 0;
            default:
                LOG (WARNING) << "\tRequested unrecognisable action!";
                return 0;
        }
        actions.insert (action);
        return now;
    }

    std::ostream & operator << (std::ostream & os, IntState const & state) {
        os << "\tState: " << state.getState() << "\n";
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
