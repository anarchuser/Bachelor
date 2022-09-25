#ifndef BACHELOR_VOTEPACKET_H
#define BACHELOR_VOTEPACKET_H

#include "Packet.h"
#include "port.h"
#include "State/Action.h"

namespace bt {
    enum Vote {
        REJECT = 0,
        APPROVE,
    };

    struct __attribute__((__packed__)) VotePacket : public Packet {
        VotePacket (port_t receiver, port_t sender, Vote vote, Action action, std::uint32_t counter);

        Vote vote;
        Action action;
    };

    std::ostream & operator << (std::ostream & os, Vote vote);
    std::ostream & operator << (std::ostream & os, VotePacket const & packet);

} // bt

#endif //BACHELOR_VOTEPACKET_H

/* Copyright © 2022 Aaron Alef */
