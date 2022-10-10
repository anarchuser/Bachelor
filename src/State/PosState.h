#ifndef BACHELOR_POSSTATE_H
#define BACHELOR_POSSTATE_H

#include <glog/logging.h>

#include <numeric>

#include "Socket/log.h"
#include "Action.h"
#include "State.h"

#include <iostream>  // remove

namespace bt {
    class PosState final : public State {
    public:
        inline explicit PosState (Position state): initialState {state} {}
        inline PosState(): PosState (Position (0, 0)) {}
        PosState (PosState const & other) = default;

        timestamp_t apply (Action action) override;
        [[nodiscard]] Position getState() const;
        [[nodiscard]] bool contains (Action action) const;

        [[nodiscard]] static bool try_apply (Action action, Position state, std::set <Action> actions);

        Position const initialState = {0, 0};
    };

    std::ostream & operator << (std::ostream & os, PosState const & state);

} // bt

#endif //BACHELOR_POSSTATE_H

/* Copyright © 2022 Aaron Alef */
