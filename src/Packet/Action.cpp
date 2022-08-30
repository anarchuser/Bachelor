#include "Action.h"

namespace bt {
    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case ALLOW_THIS:  return os << "ALLOW THIS";
            case REJECT_THIS: return os << "REJECT THIS";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
