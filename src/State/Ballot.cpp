#include "Ballot.h"

namespace bt {
    Ballot::Ballot (bt::Action action): action {action} {}

    bool Ballot::approvedBy (port_t approver) {
        if (rejecters.contains (approver)) return false;
        approvers.insert (approver);
        return true;
    }
    bool Ballot::rejectedBy (port_t rejecter) {
        if (approvers.contains (rejecter)) return false;
        rejecters.insert (rejecter);
        return true;
    }
}

/* Copyright © 2022 Aaron Alef */
