#ifndef BACHELOR_INTSTATE_H
#define BACHELOR_INTSTATE_H

#include <glog/logging.h>

#include "Action.h"
#include "State.h"

namespace bt {

    class IntState final : public State {
    public:
        IntState () = default;
        IntState (IntState const & other);

        timestamp_t init (int state);
        timestamp_t apply (Action action) override;
        [[nodiscard]] inline bool isInitialised() const { return isInit; }
        [[nodiscard]] int getState() const;

    private:
        std::atomic <bool> isInit = false;
        std::atomic <int> state = 0;
    };

    std::ostream & operator << (std::ostream & os, IntState const & state);

} // bt

#endif //BACHELOR_INTSTATE_H

/* Copyright © 2022 Aaron Alef */
