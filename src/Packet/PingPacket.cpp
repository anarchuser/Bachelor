#include "PingPacket.h"

namespace bt {
    PingPacket::PingPacket (port_t receiver, port_t sender)
            : Packet (sizeof (* this), receiver, sender, PING)
            {}

    std::ostream & operator << (std::ostream & os, bt::PingPacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        return os;
    }

    std::uint32_t PingPacket::instance_counter () const {
        thread_local std::uint32_t counter = 0;
        return counter++;
    }
}

/* Copyright © 2022 Aaron Alef */
