#ifndef BACHELOR_STATE_H
#define BACHELOR_STATE_H

#include <chrono>
#include <vector>

#include <glog/logging.h>

#include "Action.h"
#include "Chrono/util.h"

namespace bt {

    class State {
    public:
        State () = default;

        timestamp_t apply (Action action);
        [[nodiscard]] std::vector <Action> const & getActions() const;

        inline bool operator == (State const & other) const { return actions == other.actions; }

    private:
        std::vector <Action> actions;
    };

    std::ostream & operator << (std::ostream & os, State const & state);

} // bt

#endif //BACHELOR_STATE_H

/* Copyright © 2022 Aaron Alef */
