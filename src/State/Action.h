#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <glog/logging.h>
#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <utility>

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
        inline Position (std::int16_t x, std::int16_t y): x{x}, y{y} {}

        std::int16_t x = 0;
        std::int16_t y = 0;

        Position operator + (Position other) const;
        Position & operator += (Position other);
        bool operator == (Position other) const;
    };
    struct __attribute((__packed__)) PosChange {
        Position const delta;
        Position const reference;
    };

    union __attribute((__packed__)) Data {
        explicit inline Data (std::int32_t change = ACTION_DEFAULT): change {change} {}
        explicit inline Data (PosChange move): move{std::move(move)} {}

        std::int32_t const change;
        PosChange const move;
    };

    struct __attribute__((__packed__)) Action {
        Action (port_t who, ActionType what);
        Action (port_t who, std::int32_t change);
        Action (port_t who, PosChange move);

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

    std::ostream & operator << (std::ostream & os, PosChange move);
    std::ostream & operator << (std::ostream & os, Position pos);
    std::ostream & operator << (std::ostream & os, ActionType type);
    std::ostream & operator << (std::ostream & os, Action const & action);

    std::int32_t operator + (std::int32_t state, Action const & action);
    Position operator + (Position state, Action const & action);
} // bt

template<>
struct std::hash <bt::Action> {
    std::size_t  operator () (bt::Action const & action) const noexcept {
        return action.when;
    }
};

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
