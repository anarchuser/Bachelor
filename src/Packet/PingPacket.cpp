#include "PingPacket.h"

namespace bt {
    PingPacket::PingPacket (port_t receiver, port_t sender, std::uint32_t counter)
            : Packet (sizeof (* this), receiver, sender, PING, counter)
            {}

    std::ostream & operator << (std::ostream & os, bt::PingPacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
