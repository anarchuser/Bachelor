#ifndef BACHELOR_INTSTATE_H
#define BACHELOR_INTSTATE_H

#include <glog/logging.h>

#include <numeric>

#include "Socket/log.h"
#include "Action.h"
#include "State.h"

namespace bt {

    typedef std::int32_t state_t;

    class IntState final : public State {
    public:
        inline explicit IntState (state_t state): initialState {state} {}
        IntState (IntState const & other) = default;

        timestamp_t apply (Action action) override;
        [[nodiscard]] state_t getState() const;
        [[nodiscard]] bool contains (Action action) const override;

        [[nodiscard]] static bool try_apply (Action action, state_t state, std::set <Action> actions);

        state_t const initialState = 0;
    };

    std::ostream & operator << (std::ostream & os, IntState const & state);

} // bt

#endif //BACHELOR_INTSTATE_H

/* Copyright © 2022 Aaron Alef */
