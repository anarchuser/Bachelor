#ifndef BACHELOR_STATE_H
#define BACHELOR_STATE_H

#include <algorithm>
#include <chrono>
#include <mutex>
#include <map>
#include <set>
#include <unordered_set>

#include <glog/logging.h>

#include "Action.h"
#include "Chrono/util.h"

namespace bt {

    class State {
    public:
        State () = default;
        State (State const & other);

        virtual timestamp_t apply (Action action);
        [[nodiscard]] virtual std::map <Action, timestamp_t> getActions() const final;
        virtual bool contains (Action action) const = 0;
        [[nodiscard]] timestamp_t getAverageLatency() const;
        [[nodiscard]] timestamp_t getMaximumLatency() const;

        [[nodiscard]] bool operator == (State const & other) const;

    protected:
        mutable std::mutex mx;

        std::map <Action, timestamp_t> actions;

        [[nodiscard]] virtual std::map <Action, timestamp_t> getActions() final;
    };

    std::ostream & operator << (std::ostream & os, State const & state);

} // bt

#endif //BACHELOR_STATE_H

/* Copyright © 2022 Aaron Alef */
