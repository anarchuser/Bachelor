#ifndef BACHELOR_PACKET_H
#define BACHELOR_PACKET_H

#include <string>
#include <sstream>

#include <cstring>
#include <cstdint>
#include <cmath>

#include <glog/logging.h>
#include "port.h"
#include "Action.h"
#include "Chrono/util.h"

namespace bt {
    /*   576 = Minimum required size to guarantee reassembly in case of fragmentation
     * -  60 = Maximum IP header size
     * -   8 = UDP header
     * = 508 = Maximum safe UDP payload size */
    static constexpr size_t MAX_PAYLOAD_BYTES = 576 - 60 - 8;

    /* [--,--|--,--,--,--|--,--,--,--]
     * [ size| receiver  | sender    ]
     */
    struct __attribute__((__packed__)) Packet {
    public:
        size_t const size;
        port_t const receiver;
        port_t const sender;
        std::uint32_t counter;
        std::uint64_t timestamp;
        ActionType const type: 8;

        // Cast the given string to a Packet
        static Packet const & from_buffer (char const * buffer);
        // Retrieve this Packet as const char string
        [[nodiscard]] char const * c_str() const;

        virtual ~Packet() = 0;

    protected:
        Packet (size_t size, port_t receiver, port_t sender, ActionType type, std::uint32_t counter);
    };

    std::ostream & operator << (std::ostream & os, bt::Packet const & packet);
};

#endif //BACHELOR_PACKET_H

/* Copyright © 2022 Aaron Alef */
