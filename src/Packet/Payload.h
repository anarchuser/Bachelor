#ifndef BACHELOR_PAYLOAD_H
#define BACHELOR_PAYLOAD_H

#include <string>
#include <sstream>

#include <cstring>
#include <cstdint>
#include <cmath>

#include <glog/logging.h>

namespace bt {
    // TODO: put this in some config file
    typedef int32_t port_t;

    enum Action {
        PING,       // Check for life signs
        CONNECT,    // Tell of the existence of a peer
        ACK,        // Acknowledge an action (approve)
        NACK,       // Do not acknowledge an action (reject)
    };

    struct __attribute__((__packed__)) Payload {
        Action const action: 8;

        [[nodiscard]] char const * c_str() const;

        Payload() = delete;

    protected:
        explicit Payload (Action action);
    };

    struct __attribute__((__packed__)) PingPayload: public Payload {
        PingPayload();
    };

    struct __attribute__((__packed__)) ConnectPayload: public Payload {
        port_t const joiner;

        explicit ConnectPayload (port_t joiner);
    };

    std::ostream & operator << (std::ostream & os, Payload const * payload);
} // bt

#endif //BACHELOR_PAYLOAD_H

/* Copyright © 2022 Aaron Alef */
