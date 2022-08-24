#include "Action.h"

namespace bt {
    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case PING:    return os << "PING";
            case CONNECT: return os << "CONN";
            case ACTION:  return os << "ACTN";
            case ACK:     return os << "ACKN";
            case NACK:    return os << "NACK";
        }
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
