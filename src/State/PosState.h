#ifndef BACHELOR_POSSTATE_H
#define BACHELOR_POSSTATE_H

#include <glog/logging.h>

#include <numeric>

#include "Socket/log.h"
#include "Action.h"
#include "State.h"

namespace bt {

    typedef std::int32_t state_t;

    class PosState final : public State {
    public:
        inline explicit PosState (state_t state): initialState {state} {}
        PosState (PosState const & other) = default;

        timestamp_t apply (Action action) override;
        [[nodiscard]] state_t getState() const;
        [[nodiscard]] bool contains (Action action) const;

        [[nodiscard]] static bool try_apply (Action action, state_t state, std::set <Action> actions);

        state_t const initialState = 0;
    };

    std::ostream & operator << (std::ostream & os, PosState const & state);

} // bt

#endif //BACHELOR_POSSTATE_H

/* Copyright © 2022 Aaron Alef */
