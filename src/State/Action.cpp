#include "Action.h"

namespace bt {
    Action::Action (port_t who, ActionType what)
            : when {get_timestamp()}
            , who {who}
            , what {what}
            {}

    Action::Action (port_t who, std::int32_t value)
            : when {get_timestamp()}
            , who {who}
            , what {ADD}
            , value {value}
            {}

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case NOOP:      return os << "NOOP";
            case FORBIDDEN: return os << "FORBIDDEN";
            case ADD:       return os << "ADD";
            default:        return os << "[UNKNOWN]";
        }
    }

    std::ostream & operator << (std::ostream & os, Action const & action) {
        os << action.who;
        if (action.what == ADD) {
            os << "|" << action.what << "(" << action.value << ")";
        }
        os << "|@" << action.when;
        return os;
    }

    std::int32_t operator + (std::int32_t state, Action const & action) {
        switch (action.what) {
            case FORBIDDEN:
                LOG (WARNING) << "\tTrying to apply a forbidden action!";
            case NOOP:
                return state;
            case ADD:
                return state + action.value;
            default:
                LOG (WARNING) << "\tTrying to apply an unrecognisable action!";
                return state;
        }
    }
}

/* Copyright © 2022 Aaron Alef */
