#ifndef BACHELOR_INTSTATE_H
#define BACHELOR_INTSTATE_H

#include <glog/logging.h>

#include <numeric>

#include "Action.h"
#include "State.h"

namespace bt {

    typedef std::int32_t state_t;

    class IntState final : public State {
    public:
        inline IntState (state_t state): state {state} {}
        IntState (IntState const & other);

        timestamp_t apply (Action action) override;
        [[nodiscard]] state_t getState() const;

    private:
        state_t const state = 0;
    };

    std::ostream & operator << (std::ostream & os, IntState const & state);

} // bt

#endif //BACHELOR_INTSTATE_H

/* Copyright © 2022 Aaron Alef */
