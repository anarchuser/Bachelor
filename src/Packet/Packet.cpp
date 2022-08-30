#include "Packet.h"

namespace bt {
    Packet::Packet (size_t size, port_t receiver, port_t sender, PacketType type, std::uint32_t counter)
            : size {size}
            , receiver {receiver}
            , sender {sender}
            , counter {counter}
            , timestamp {get_timestamp()}
            , type {type}
            {}

    Packet::~Packet () = default;

    Packet const & Packet::from_buffer (char const * buffer) {
        return * (Packet const *) buffer;
    }
    char const * Packet::c_str () const {
        return (char const *) this;
    }

    std::ostream & operator << (std::ostream & os, PacketType type) {
        switch (type) {
            case PING:    return os << "PING";
            case CONNECT: return os << "CONN";
            case ACTION:  return os << "ACTN";
            case ACK:     return os << "ACKN";
            case NACK:    return os << "NACK";
        }
        return os;
    }

    std::ostream & operator << (std::ostream & os, bt::Packet const & packet) {
        os <<        packet.size;
        os << "|" << packet.receiver;
        os << "|" << packet.sender;
        os << "|" << std::setfill('0') << std::setw (5) << packet.counter;
        os << "|" << packet.type;
        return os;
    }
}

/* Copyright © 2022 Aaron Alef */
