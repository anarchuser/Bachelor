#ifndef BACHELOR_ACTIONPACKET_H
#define BACHELOR_ACTIONPACKET_H

#include "Packet.h"
#include "port.h"
#include "Action.h"

namespace bt {

    struct __attribute__((__packed__)) ActionPacket : public Packet {
    public:
        ActionPacket (port_t receiver, port_t sender, std::uint32_t counter);
    };

    std::ostream & operator << (std::ostream & os, bt::ActionPacket const & packet);

} // bt

#endif //BACHELOR_ACTIONPACKET_H

/* Copyright © 2022 Aaron Alef */
