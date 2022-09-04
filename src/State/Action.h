#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <glog/logging.h>
#include <sstream>

#include "Packet/port.h"
#include "Chrono/util.h"

#define ACTION_DEFAULT 0

namespace bt {
    enum ActionType {
        NOOP,
        FORBIDDEN,
        ADD,
    };

    struct __attribute__((__packed__)) Action {
        Action (port_t who, ActionType what);
        Action (port_t who, std::int32_t value);

        timestamp_t const when;
        port_t const who;
        ActionType const what: 8;
        std::int32_t const value = ACTION_DEFAULT;

        inline bool operator == (Action const & other) const {
            return when == other.when && who == other.who && what == other.what;
        }
        inline bool operator <  (Action const & other) const {
            return when < other.when;
        }
    };

    std::ostream & operator << (std::ostream & os, ActionType type);
    std::ostream & operator << (std::ostream & os, Action const & action);

    std::int32_t operator + (std::int32_t state, Action const & action);
} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
