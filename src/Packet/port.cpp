#include "port.h"

namespace bt {
    std::string addr2str (in_addr_t address) {
        std::stringstream ss;
        auto bytes = (unsigned char const *) & address;
        ss << (int) bytes[0] << ".";
        ss << (int) bytes[1] << ".";
        ss << (int) bytes[2] << ".";
        ss << (int) bytes[3];
        return ss.str();
    }
    std::string addr2str (in_addr_t address, port_t port) {
        return addr2str (address) + ":" + std::to_string (port);
    }
}

/* Copyright © 2022 Aaron Alef */
