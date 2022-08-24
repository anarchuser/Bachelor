#ifndef BACHELOR_PINGPACKET_H
#define BACHELOR_PINGPACKET_H

#include "Packet.h"
#include "port.h"

namespace bt {

    struct __attribute__((__packed__)) PingPacket : public Packet {
    public:
        PingPacket (port_t receiver, port_t sender, std::uint32_t counter);
    };

    std::ostream & operator << (std::ostream & os, bt::PingPacket const & packet);

} // bt

#endif //BACHELOR_PINGPACKET_H

/* Copyright © 2022 Aaron Alef */
