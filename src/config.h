#ifndef BACHELOR_CONFIG_H
#define BACHELOR_CONFIG_H

#define PORT_ROUTER_OUT 49999
#define PORT_ROUTER     50000
#define PORT_PEER_START 50001
#define PORT_PEER_END   60000

#include <atomic>

static std::atomic <int> port_index = PORT_PEER_START;

#endif //BACHELOR_CONFIG_H

/* Copyright © 2022 Aaron Alef */
