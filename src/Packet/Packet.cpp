#include "Packet.h"

namespace bt {
    Packet::Packet (port_t receiver, port_t sender, std::string const & msg)
            : header { std::min (size_t (MAX_PAYLOAD_BYTES - 1), size_t (sizeof (header) + msg.length()))
                     , receiver
                     , sender
                     }
            {
                if (msg.length() >= MAX_CONTENT_BYTES) {
                    LOG (WARNING) << "Message exceeds maximum safe payload of " << MAX_CONTENT_BYTES << " bytes";
                }

                auto msg_size = std::min (MAX_CONTENT_BYTES, size_t (msg.length()));
                msg.copy (payload, msg_size);
                payload [msg_size] = 0;
            }

    Packet const & Packet::from_buffer (char const * buffer) {
        return * (Packet const *) buffer;
    }
    char const * Packet::c_str () const {
        return (char const *) this;
    }

    std::ostream  & operator << (std::ostream & os, bt::Packet const & packet) {
        os << '[' << packet.header.size;
        os << '|' << packet.header.receiver;
        os << '|' << packet.header.sender;
        os << '|' << (Payload const *) packet.payload;
        os << ']';
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
