#ifndef BACHELOR_UTIL_H
#define BACHELOR_UTIL_H

#include <cstdint>
#include <chrono>
#include <locale>
#include <string>
#include <sstream>

#define SEPARATE_THOUSANDS

namespace bt {
    typedef std::uint64_t timestamp_t;

    std::string separate_thousands (timestamp_t timestamp);
    timestamp_t get_timestamp();
    std::string get_time_string();
}

#endif //BACHELOR_UTIL_H

/* Copyright © 2022 Aaron Alef */
