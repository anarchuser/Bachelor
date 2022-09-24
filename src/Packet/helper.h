#ifndef BACHELOR_ARGS_H
#define BACHELOR_HELPER_H

#include "Packet.h"
#include "PingPacket.h"
#include "ConnectPacket.h"
#include "VotePacket.h"

#define DISPATCH(function, packet)                                              \
    switch (packet.type) {                                                      \
        case PING:                                                              \
            function (dynamic_cast <PingPacket const &> (packet)); break;       \
        case CONNECT:                                                           \
            function (dynamic_cast <ConnectPacket const &> (packet)); break;    \
        case VOTE:                                                              \
            function (dynamic_cast <VotePacket const &> (packet)); break;       \
        default:                                                                \
            LOG (WARNING) << PRINT_PORT << "Cannot handle type " << packet.type;\
    }; false


namespace bt {
    std::string to_string (Packet const & packet);
}

#endif //BACHELOR_ARGS_H

/* Copyright © 2022 Aaron Alef */
