#ifndef BACHELOR_PACKET_H
#define BACHELOR_PACKET_H

#include <string>
#include <sstream>

#include <cstring>
#include <cstdint>
#include <cmath>

#include <glog/logging.h>

namespace bt {
    typedef int32_t port_t;
    typedef uint16_t size_t;

    enum Action {
        PING,       // Check for life signs
        CONNECT     // Tell of the existence of a peer
    };

    /*   576 = Minimum required size to guarantee reassembly in case of fragmentation
     * -  60 = Maximum IP header size
     * -   8 = UDP header
     * = 508 = Maximum safe UDP payload size */
    static constexpr size_t MAX_PAYLOAD_BYTES = 576 - 60 - 8;

    /* [--,--|--,--,--,--|--,--,--,--|--,--,...]
     * [ size| receiver  | sender    | content ]
     */
    struct __attribute__((__packed__)) Packet {
        struct __attribute__((__packed__)) Header {
            size_t const size;
            port_t const receiver;
            port_t const sender;
            Action const action = CONNECT;
        } header;
        char content [MAX_PAYLOAD_BYTES - sizeof (Header)];

        Packet (port_t receiver, port_t sender, std::string const & msg);

        /// Interpret 0-terminated string as Packet
        static Packet const & from_buffer (char const * buffer);
        /// Retrieve this Packet as 0-terminated string
        [[nodiscard]] char const * c_str() const;

//        std::ostream & operator << (std::ostream & os) const;
    };

    static constexpr size_t MAX_CONTENT_BYTES = MAX_PAYLOAD_BYTES - sizeof (Packet::Header);

    std::ostream & operator << (std::ostream & os, bt::Packet const & packet);

    static_assert (sizeof (Packet) == MAX_PAYLOAD_BYTES);
};


#endif //BACHELOR_PACKET_H

/* Copyright © 2022 Aaron Alef */
