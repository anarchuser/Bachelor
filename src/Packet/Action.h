#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <sstream>

namespace bt {
    enum ActionType {
        PING,       // Check for life signs
        CONNECT,    // Tell of the existence of a peer
        ACTION,     // Perform an action
        ACK,        // Acknowledge an action (approve)
        NACK,       // Do not acknowledge an action (reject)
    };

    std::ostream & operator << (std::ostream & os, ActionType type);

    class Action {

    };

} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
