#include "util.h"

namespace bt {
    std::size_t get_timestamp() {
        return std::chrono::duration_cast <std::chrono::nanoseconds> (
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}

/* Copyright © 2022 Aaron Alef */
