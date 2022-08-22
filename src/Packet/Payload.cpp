#include "Payload.h"

namespace bt {
    char const * Payload::c_str () const {
        return (char const *) this;
    }

    Payload::Payload (ActionType action)
            : type {action}
            , timestamp {get_timestamp()}
            {}

    PingPayload::PingPayload ()
            : Payload (PING)
            {}

    ConnectPayload::ConnectPayload (bt::port_t joiner)
            : Payload (CONNECT)
            , joiner {joiner}
            {}

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case PING:    return os << "PING";
            case CONNECT: return os << "CONN";
            case ACK:     return os << "ACKN";
            case NACK:    return os << "NACK";
        }
        return os;
    }

    std::ostream & operator << (std::ostream & os, Payload const * payload) {
        os << "@" << payload->timestamp << "|";
        os << "Δ" << get_timestamp() - payload->timestamp << "ns|";
        os << payload->type;
        switch (payload->type) {
            case PING:
                return os;
            case CONNECT:
                return os << "|" << ((ConnectPayload const *) payload)->joiner;
            default:
                return os;
        }
    }

    std::size_t get_timestamp() {
        return std::chrono::duration_cast <std::chrono::nanoseconds> (
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    ActionType Payload::get_type (char const * buffer) {
        auto const * payload = (Payload const *) buffer;
        return payload->type;
    }
}

/* Copyright © 2022 Aaron Alef */
