#ifndef BACHELOR_SOCKET_H
#define BACHELOR_SOCKET_H

#include <glog/logging.h>
#include <iostream>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>

#include "Packet/Packet.h"

namespace bt {
    struct Socket {
    public:
        port_t const port;

        explicit Socket (port_t port);
        ~Socket();

        [[noreturn]] virtual void service () final;

        virtual void send (Packet const & packet, port_t receiver);
        virtual void send (Packet const & packet);

    protected:
        virtual void process (Packet const & packet, port_t sender);

    private:
        struct sockaddr_in address = {0};
        int const socket_fd;
    };
}

#endif //BACHELOR_SOCKET_H

/* Copyright © 2022 Aaron Alef */
