#include "IntState.h"

namespace bt {
    IntState::IntState (IntState const & other): State (other) {
        std::lock_guard other_lock (other.mx);
        std::lock_guard own_lock (mx);
        isInit = other.isInit.load();
        state = other.state.load();
    }

    timestamp_t IntState::init (int state) {
        if (isInit) {
            LOG (WARNING) << "\tTried to overwrite already initialised state!";
            return 0;
        }

        std::lock_guard guard (mx);
        auto now = get_timestamp();
        isInit = true;
        this->state = state;
        return now;
    }

    timestamp_t IntState::apply (Action action) {
        if (!isInit) {
            LOG (WARNING) << "\tRequested action before State got initialised!";
            return 0;
        }

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
        actions.emplace_back (action);
        return now;
    }

    int IntState::getState () const {
        LOG_IF (WARNING, isInit) << "\tRequesting State before it got initialised!";
        return state;
    }

    std::ostream & operator << (std::ostream & os, IntState const & state) {
        if (!state.isInitialised()) {
            return os << "\t[UNINITIALISED]\n";
        }
        os << "\tState: " << state.getState();
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
