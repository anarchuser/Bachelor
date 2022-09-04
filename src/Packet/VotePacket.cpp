#include "VotePacket.h"

namespace bt {
    VotePacket::VotePacket (port_t receiver, port_t sender, Vote vote, Action action, std::uint32_t counter)
            : Packet (sizeof (* this), receiver, sender, VOTE, counter)
            , vote {vote}
            , action {action}
            {}

    std::ostream & operator << (std::ostream & os, Vote vote) {
        switch (vote) {
            case APPROVE: return os << "APPROVE";
            case REJECT:  return os << "REJECT";
            default:      return os;
        }
    }
    std::ostream & operator << (std::ostream & os, VotePacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        os << "|" << packet.vote;
        os << packet.action;
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
