#ifndef BACHELOR_STATE_H
#define BACHELOR_STATE_H

#include <chrono>
#include <mutex>
#include <set>

#include <glog/logging.h>

#include "Action.h"
#include "Chrono/util.h"

namespace bt {

    class State {
    public:
        State () = default;
        State (State const & other);

        virtual timestamp_t apply (Action action);
        [[nodiscard]] virtual std::set <Action> const & getActions() const final;

        inline bool operator == (State const & other) const { return actions == other.actions; }

    protected:
        mutable std::mutex mx;

        std::set <Action> actions;
    };

    std::ostream & operator << (std::ostream & os, State const & state);

} // bt

#endif //BACHELOR_STATE_H

/* Copyright © 2022 Aaron Alef */
