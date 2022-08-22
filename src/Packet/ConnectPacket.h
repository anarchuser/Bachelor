#ifndef BACHELOR_CONNECTPACKET_H
#define BACHELOR_CONNECTPACKET_H

#include "Packet.h"
#include "port.h"

namespace bt {

    struct ConnectPacket : public Packet {
    public:
        ConnectPacket (port_t receiver, port_t sender, port_t joiner);

        port_t const joiner;
    };

    std::ostream & operator << (std::ostream & os, bt::ConnectPacket const & packet);

} // bt

#endif //BACHELOR_CONNECTPACKET_H

/* Copyright © 2022 Aaron Alef */
