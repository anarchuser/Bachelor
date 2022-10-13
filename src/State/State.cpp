#include "State.h"

namespace bt {
    State::State (State const & other): actions {other.getActions()} {}

    timestamp_t State::apply (Action action) {
        std::lock_guard guard (mx);
        timestamp_t now = get_timestamp();
        actions.emplace (action, now);
        return now;
    }

    std::map <Action, timestamp_t> State::getActions() {
        std::map <Action, timestamp_t> copy;
        for (auto action : actions) {
            copy.insert (action);
        }
        return copy;
    }
    std::map <Action, timestamp_t> State::getActions() const {
        std::lock_guard guard (mx);
        std::map <Action, timestamp_t> copy;
        for (auto action : actions) {
            copy.insert (action);
        }
        return copy;
    }

    timestamp_t State::getAverageLatency () const {
        if (actions.empty()) return 0;
        auto total_latency = std::accumulate (actions.begin(), actions.end(), 0,
                [] (timestamp_t latency, std::pair <Action, timestamp_t> const & action) {
                    LOG_IF (WARNING, action.second < action.first.when) << "Action registered before it supposedly happened! - " << action.first;
                    return latency + action.second - action.first.when;
        });
        return total_latency / actions.size();
    }

    timestamp_t State::getMaximumLatency() const {
        if (actions.empty()) return 0;
        return std::accumulate (actions.begin(), actions.end(), 0,
                [] (timestamp_t latency, std::pair <Action, timestamp_t> const & action) {
                    LOG_IF (WARNING, action.second < action.first.when) << "Action registered before it supposedly happened! - " << action.first;
                    return std::max (latency, action.second - action.first.when);
        });
    }

    bool State::operator == (State const & other) const {
        auto const & other_actions = other.getActions();
        return std::all_of (
                other_actions.begin(),
                other_actions.end(),
                [&](std::pair <Action, timestamp_t> const & other_action) {
                    return actions.contains (other_action.first);
                });
    }

    std::ostream & operator << (std::ostream & os, State const & state) {
        if (state.getActions().empty()) {
            return os << "\t[UNINITIALISED]\n";
        }
        for (auto action : state.getActions()) {
            os << "\t" << action.first.who;
//            os << " @" << timestamp;
            os << ": " << action.first.what;
            os << " @" << action.first.when;
            os << "\n";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
