#include "Packet.h"

namespace bt {
    Packet::Packet (int receiver, int sender, std::string const & msg)
            : header { std::min (size_t (MAX_PAYLOAD_BYTES - 1), size_t (sizeof (Header) + msg.length()))
                     , receiver
                     , sender
                     }
            {
                if (msg.length() >= MAX_CONTENT_BYTES) {
                    LOG (WARNING) << "Message exceeds maximum safe content of " << MAX_CONTENT_BYTES << " bytes";
                }

                auto msg_size = std::min (MAX_CONTENT_BYTES, size_t (msg.length()));
                msg.copy (content, msg_size);
                content [msg_size] = 0;
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
        os << '|' << packet.content;
        os << ']';
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
