#ifndef BACHELOR_HELPER_H
#define BACHELOR_HELPER_H

#include "Packet.h"
#include "PingPacket.h"
#include "ConnectPacket.h"
#include "ActionPacket.h"

#define DISPATCH(function, packet)                                              \
    switch (packet.type) {                                                      \
        case PING:                                                              \
            function (dynamic_cast <PingPacket const &> (packet)); break;           \
        case CONNECT:                                                           \
            function (dynamic_cast <ConnectPacket const &> (packet)); break;        \
        case ACTION:                                                            \
            function (dynamic_cast <ActionPacket const &> (packet)); break;         \
        case ACK:                                                               \
        case NACK:                                                              \
        default:                                                                \
            LOG (WARNING) << PRINT_PORT << "Cannot handle type " << packet.type;\
    }; false


namespace bt {
    std::string to_string (Packet const & packet);
}

#endif //BACHELOR_HELPER_H

/* Copyright © 2022 Aaron Alef */
