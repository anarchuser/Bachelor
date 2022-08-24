#include "Packet.h"

namespace bt {
    Packet::Packet (size_t size, port_t receiver, port_t sender, ActionType type)
            : size {size}
            , receiver {receiver}
            , sender {sender}
            , type {type}
            , timestamp {get_timestamp()}
            {}

    Packet::~Packet () = default;

    Packet const & Packet::from_buffer (char const * buffer) {
        return * (Packet const *) buffer;
    }
    char const * Packet::c_str () const {
        return (char const *) this;
    }

    std::ostream & operator << (std::ostream & os, bt::Packet const & packet) {
        os << packet.size     << "|";
        os << packet.receiver << "|";
        os << packet.sender   << "|";
        os << packet.type;
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
