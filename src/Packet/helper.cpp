#include "helper.h"

namespace bt {
    std::string to_string (Packet const & packet) {
        std::ostringstream ss;
        switch (packet.type) {
            case PING:    ss << * (PingPacket const *) & packet; break;
            case CONNECT: ss << * (ConnectPacket const *) & packet; break;
            case VOTE:    ss << * (VotePacket const *) & packet; break;
            default:      ss << packet;
        }
        return ss.str();
    }
}

/* Copyright © 2022 Aaron Alef */
