#include "helper.h"

namespace bt {
    std::string to_string (Packet const & packet) {
        std::ostringstream ss;
        switch (packet.type) {
            case PING:    ss << dynamic_cast <PingPacket const &> (packet); break;
            case CONNECT: ss << dynamic_cast <ConnectPacket const &> (packet); break;
            case ACTION:  ss << dynamic_cast <ActionPacket const &> (packet); break;
            default:      ss << packet;
        }
        return ss.str();
    }
}

/* Copyright © 2022 Aaron Alef */
