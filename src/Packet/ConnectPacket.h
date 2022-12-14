#ifndef BACHELOR_CONNECTPACKET_H
#define BACHELOR_CONNECTPACKET_H

#include "Packet.h"
#include "port.h"

namespace bt {

    struct __attribute__((__packed__)) ConnectPacket : public Packet {
    public:
        ConnectPacket (port_t receiver, port_t sender, port_t joiner, std::uint32_t counter);

        port_t const joiner;
    };

    std::ostream & operator << (std::ostream & os, bt::ConnectPacket const & packet);

} // bt

#endif //BACHELOR_CONNECTPACKET_H

/* Copyright © 2022 Aaron Alef */
