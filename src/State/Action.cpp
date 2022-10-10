#include "Action.h"

namespace bt {
    Action::Action (port_t who, ActionType what)
            : when {get_timestamp()}
            , who {who}
            , what {what}
            {}

    Action::Action (port_t who, std::int32_t change)
            : when {get_timestamp()}
            , who {who}
            , what {ADD}
            , value {change}
            {}

    Action::Action (port_t who, Position move)
            : when {get_timestamp()}
            , who {who}
            , what {MOVE}
            , value {move}
            {}

    std::ostream & operator << (std::ostream & os, Position move) {
        return os << "(" << move.x << "->" << move.x + move.dx << "|" << move.y << "->" << move.y + move.dy << ")";
    }

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case NOOP:      return os << "NOOP";
            case FORBIDDEN: return os << "FORBIDDEN";
            case ADD:       return os << "ADD";
            case MOVE:      return os << "MOVE";
            default:        return os << "[UNKNOWN]";
        }
    }

    std::ostream & operator << (std::ostream & os, Action const & action) {
        os << "[" << action.who;
        os << "|" << action.what;
        if (action.what == ADD) {
            os << "(" << std::right << std::setfill (' ') << std::setw(3) << action.value.change << ")";
        } else if (action.what == MOVE) {
            os << "(" << std::right << std::setfill (' ') << std::setw(3) << action.value.move << ")";
        }
        os << "|@" << action.when;
        return os << "]";
    }

    std::int32_t operator + (std::int32_t state, Action const & action) {
        switch (action.what) {
            case FORBIDDEN:
                LOG (WARNING) << "\tTrying to apply a forbidden action!";
            case NOOP:
                return state;
            case ADD:
                return state + action.value.change;
            case MOVE:
                return state;
            default:
                LOG (WARNING) << "\tTrying to apply an unrecognisable action!";
                return state;
        }
    }
}

/* Copyright © 2022 Aaron Alef */
