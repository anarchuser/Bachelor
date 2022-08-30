#include "util.h"

namespace bt {
    timestamp_t get_timestamp() {
        return std::chrono::duration_cast <std::chrono::nanoseconds> (
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    std::string separate_thousands (timestamp_t timestamp) {
        std::ostringstream oss;
#ifdef SEPARATE_THOUSANDS
        oss.imbue (std::locale (""));
#endif
        oss << timestamp;
        return oss.str();
    }

    std::string get_time_string() {
        return separate_thousands (get_timestamp());
    }
}

/* Copyright © 2022 Aaron Alef */
