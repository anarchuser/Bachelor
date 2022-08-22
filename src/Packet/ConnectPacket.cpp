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
}

/* Copyright © 2022 Aaron Alef */
