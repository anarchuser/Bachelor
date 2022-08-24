#include "ConnectPacket.h"

namespace bt {
    ConnectPacket::ConnectPacket (port_t receiver, port_t sender, port_t joiner, std::uint32_t counter)
            : Packet (sizeof (* this), receiver, sender, CONNECT, counter)
            , joiner {joiner}
            {}

    std::ostream & operator << (std::ostream & os, bt::ConnectPacket const & packet) {
        os << dynamic_cast <Packet const &> (packet);
        os << "|" << packet.joiner;
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
