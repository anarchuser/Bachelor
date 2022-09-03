#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <sstream>

#include "Packet/port.h"
#include "Chrono/util.h"

namespace bt {
    enum ActionType {
        NOOP,
        FORBIDDEN,
    };

    struct __attribute__((__packed__)) Action {
        Action (port_t who, ActionType what);

        timestamp_t const when;
        port_t const who;
        ActionType const what: 8;

        inline bool operator == (Action const & other) const {
            return when == other.when && who == other.who && what == other.what;
        }
        inline bool operator <  (Action const & other) const {
            return when < other.when;
        }
    };

    std::ostream & operator << (std::ostream & os, ActionType type);
    std::ostream & operator << (std::ostream & os, Action const & action);
} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
