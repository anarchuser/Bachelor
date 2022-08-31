#include "Action.h"

namespace bt {
    Action::Action (port_t who, ActionType what)
            : when {get_timestamp()}
            , who {who}
            , what {what}
            {}

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case ALLOW_THIS:  return os << "ALLOW THIS";
            case REJECT_THIS: return os << "REJECT THIS";
        }
        return os;
    }

    std::ostream & operator << (std::ostream & os, Action const & action) {
        os << action.who;
        os << "|@" << action.when;
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
