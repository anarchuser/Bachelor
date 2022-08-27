#ifndef BACHELOR_PORT_H
#define BACHELOR_PORT_H

#include <cstdint>
#include <netinet/in.h>
#include <sstream>

namespace bt {
    typedef std::uint16_t port_t;
    typedef std::uint16_t size_t;

    std::string addr2str (in_addr_t address);
    std::string addr2str (in_addr_t address, port_t port);
}

#endif //BACHELOR_PORT_H

/* Copyright © 2022 Aaron Alef */
