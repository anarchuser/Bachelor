#include "Action.h"

namespace bt {
    Action::Action (port_t who, ActionType what, std::int32_t value)
            : when {get_timestamp()}
            , who {who}
            , what {what}
            , value {value}
            {}

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case NOOP:  return os << "NOOP";
            case FORBIDDEN: return os << "FORBIDDEN";
        }
        return os;
    }

    std::ostream & operator << (std::ostream & os, Action const & action) {
        os << action.who;
        os << "|@" << action.when;
        return os;
    }

    std::int32_t operator + (std::int32_t state, Action const & action) {
        switch (action.what) {
            case FORBIDDEN:
                LOG (WARNING) << "\tTrying to apply a forbidden action!";
            case NOOP:
                return state;
            default:
                LOG (WARNING) << "\tTrying to apply an unrecognisable action!";
                return state;
        }
    }
}

/* Copyright © 2022 Aaron Alef */
