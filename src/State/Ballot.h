#ifndef BACHELOR_BALLOT_H
#define BACHELOR_BALLOT_H

#include <unordered_set>

#include "Action.h"
#include "Packet/port.h"

namespace bt {

    struct Ballot {
        explicit Ballot (Action action);

        Action const action;
        std::unordered_set <port_t> approvers;
        std::unordered_set <port_t> rejecters;

        bool approvedBy (port_t approver);
        bool rejectedBy (port_t rejecter);
    };

} // bt

#endif //BACHELOR_BALLOT_H

/* Copyright © 2022 Aaron Alef */
