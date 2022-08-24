#include "ActionPacket.h"

namespace bt {
    ActionPacket::ActionPacket (port_t receiver, port_t sender, std::uint32_t counter)
            : Packet (sizeof (* this), receiver, sender, ACTION, counter)
            {}

    std::ostream & operator << (std::ostream & os, bt::ActionPacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
