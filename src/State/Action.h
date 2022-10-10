#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <glog/logging.h>
#include <sstream>
#include <iomanip>

#include "Packet/port.h"
#include "Chrono/util.h"

#define ACTION_DEFAULT 0

namespace bt {
    enum ActionType {
        NOOP,
        FORBIDDEN,
        ADD,
        MOVE,
    };

    struct __attribute((__packed__)) Position {
        std::int16_t const x = 0;
        std::int16_t const y = 0;
        std::int8_t const dx = 0;
        std::int8_t const dy = 0;
    };

    union __attribute((__packed__)) Data {
        explicit inline Data (std::int32_t change = ACTION_DEFAULT): change {change} {}
        explicit inline Data (Position move): move{move} {}

        std::int32_t const change;
        Position const move;
    };

    struct __attribute__((__packed__)) Action {
        Action (port_t who, ActionType what);
        Action (port_t who, std::int32_t change);
        Action (port_t who, Position move);

        timestamp_t const when;
        port_t const who;
        ActionType const what: 8;
        Data const value;

        inline bool operator == (Action const & other) const {
            return when == other.when && who == other.who && what == other.what;
        }
        inline bool operator <  (Action const & other) const {
            return when < other.when;
        }
    };

    std::ostream & operator << (std::ostream & os, Position move);
    std::ostream & operator << (std::ostream & os, ActionType type);
    std::ostream & operator << (std::ostream & os, Action const & action);

    std::int32_t operator + (std::int32_t state, Action const & action);
} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
