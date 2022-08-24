#include "ConnectPacket.h"

namespace bt {
    ConnectPacket::ConnectPacket (port_t receiver, port_t sender, port_t joiner)
            : Packet (sizeof (* this), receiver, sender, CONNECT)
            , joiner {joiner}
            {}

    std::ostream & operator << (std::ostream & os, bt::ConnectPacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        os << "|" << packet.joiner;
        return os;
    }

    std::uint32_t ConnectPacket::instance_counter () const {
        thread_local std::uint32_t counter = 0;
        return counter++;
    }
}

/* Copyright © 2022 Aaron Alef */
