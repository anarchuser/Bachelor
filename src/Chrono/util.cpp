#include "util.h"

namespace bt {
    std::string separate_thousands (timestamp_t timestamp) {
        std::stringstream ss;
#ifdef SEPARATE_THOUSANDS
        ss.imbue (std::locale (""));
#endif
        ss << timestamp;
        return ss.str();
    }

    timestamp_t get_timestamp() {
        return std::chrono::duration_cast <std::chrono::nanoseconds> (
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    std::string get_time_string() {
        return separate_thousands (get_timestamp());
    }
}

/* Copyright © 2022 Aaron Alef */
