#ifndef BACHELOR_UUID_H
#define BACHELOR_UUID_H

#include <cstdint>

#include "Action.h"
#include "Packet.h"
#include "Payload.h"
#include "port.h"

namespace bt {
    struct __attribute__((__packed__)) UUID {
        static UUID instance (Packet const & packet);

        [[nodiscard]] char const * c_str() const;
        [[nodiscard]] std::uint64_t uuid() const;

        size_t packet_num;
        port_t receiver;
        port_t sender;
        ActionType type: 8;
    private:
        explicit UUID (Packet const & packet, size_t num);

        char const _ = 0;
    };

    static_assert (sizeof (UUID) == sizeof (std::uint64_t));

    std::ostream & operator << (std::ostream & os, UUID const & uuid);
};

#endif //BACHELOR_UUID_H

/* Copyright © 2022 Aaron Alef */
