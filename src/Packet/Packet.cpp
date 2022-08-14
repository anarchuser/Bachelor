#include "Packet.h"

namespace bt {
    Packet::Packet (char const * msg)
            : receiver {* (int *) msg}
            , sender {* (int *) (msg + sizeof (int))}
            , msg {msg + 2 * sizeof (int)} {}

    char const * Packet::c_str() const {
        return (char const *) this;
    }
}

/* Copyright © 2022 Aaron Alef */
