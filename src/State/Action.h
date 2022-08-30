#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <sstream>

#include "Packet/port.h"
#include "Chrono/util.h"

namespace bt {
    enum ActionType {
        ALLOW_THIS,
        REJECT_THIS,
    };

    struct __attribute__((__packed__)) Action {
        timestamp_t const timestamp;
        port_t const port;
        ActionType const type: 8;
    };

    std::ostream & operator << (std::ostream & os, ActionType type);
    std::ostream & operator << (std::ostream & os, Action const & action);
} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
