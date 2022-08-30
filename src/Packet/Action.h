#ifndef BACHELOR_ACTION_H
#define BACHELOR_ACTION_H

#include <sstream>

namespace bt {
    enum ActionType {
        ALLOW_THIS,
        REJECT_THIS,
    };

    std::ostream & operator << (std::ostream & os, ActionType type);

    class Action {

    };

} // bt

#endif //BACHELOR_ACTION_H

/* Copyright © 2022 Aaron Alef */
