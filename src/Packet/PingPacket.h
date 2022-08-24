#ifndef BACHELOR_PINGPACKET_H
#define BACHELOR_PINGPACKET_H

#include "Packet.h"
#include "port.h"

namespace bt {

    struct PingPacket : public Packet {
    public:
        PingPacket (port_t receiver, port_t sender);

    private:
        [[nodiscard]] std::uint32_t instance_counter() const override;
    };

    std::ostream & operator << (std::ostream & os, bt::PingPacket const & packet);

} // bt

#endif //BACHELOR_PINGPACKET_H

/* Copyright © 2022 Aaron Alef */
