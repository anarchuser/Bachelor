#ifndef BACHELOR_PAYLOAD_H
#define BACHELOR_PAYLOAD_H

#include <string>
#include <sstream>

#include <cstring>
#include <cstdint>
#include <cmath>

#include <glog/logging.h>

#include "port.h"

#include "Action.h"

namespace bt {

    struct __attribute__((__packed__)) Payload {
        std::uint64_t timestamp;
        ActionType const type: 8;

        [[nodiscard]] char const * c_str() const;

        static ActionType get_type (char const * buffer);

        Payload() = delete;

    protected:
        explicit Payload (ActionType action);
    };

    struct __attribute__((__packed__)) PingPayload: public Payload {
        PingPayload();
    };

    struct __attribute__((__packed__)) ConnectPayload: public Payload {
        port_t const joiner;

        explicit ConnectPayload (port_t joiner);
    };

    std::ostream & operator << (std::ostream & os, Payload const * payload);

    std::size_t get_timestamp();
} // bt

#endif //BACHELOR_PAYLOAD_H

/* Copyright © 2022 Aaron Alef */
