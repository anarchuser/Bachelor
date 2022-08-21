#include "Payload.h"

namespace bt {
    char const * Payload::c_str () const {
        return (char const *) this;
    }

    Payload::Payload (Action action)
            : action {action}
            {}

    PingPayload::PingPayload ()
            : Payload (PING)
            {}

    ConnectPayload::ConnectPayload (bt::port_t joiner)
            : Payload (CONNECT)
            , joiner {joiner}
            {}

    std::ostream & operator << (std::ostream & os, Payload const * payload) {
        switch (payload->action) {
            case PING:
                return os << "PING";
            case CONNECT:
                return os << "JOIN|" << ((ConnectPayload const *) payload)->joiner;
            default:
                return os;
        }
    }
}

/* Copyright © 2022 Aaron Alef */
