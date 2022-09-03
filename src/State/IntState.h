#ifndef BACHELOR_INTSTATE_H
#define BACHELOR_INTSTATE_H

#include <glog/logging.h>

#include "Action.h"
#include "State.h"

namespace bt {

    class IntState final : public State {
    public:
        inline IntState (int state): state {state} {}
        IntState (IntState const & other);

        timestamp_t apply (Action action) override;
        [[nodiscard]] inline int getState() const { return state; }

    private:
        std::atomic <int> state = 0;
    };

    std::ostream & operator << (std::ostream & os, IntState const & state);

} // bt

#endif //BACHELOR_INTSTATE_H

/* Copyright © 2022 Aaron Alef */
