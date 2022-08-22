#include "uuid.h"

namespace bt {
    UUID UUID::instance (bt::Packet const & packet) {
        thread_local size_t counter = 0;
        return UUID (packet, counter++);
    }

    UUID::UUID (Packet const & packet, size_t num)
            : packet_num {num}
            , receiver {packet.header.receiver}
            , sender {packet.header.sender}
            , type {Payload::get_type (packet.payload)}
    {}

    char const * UUID::c_str () const {
        return (char const*) this;
    }

    std::uint64_t UUID::uuid () const {
        return * (std::uint64_t const *) this;
    }

    std::ostream & operator << (std::ostream & os, UUID const & uuid) {
        os << "#" << uuid.packet_num << "/";
        os << "R" << uuid.receiver << "/";
        os << "S" << uuid.sender << "/";
        return os << uuid.type;
    }
}

/* Copyright © 2022 Aaron Alef */
