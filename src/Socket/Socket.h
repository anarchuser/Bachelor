#ifndef BACHELOR_SOCKET_H
#define BACHELOR_SOCKET_H

#include <glog/logging.h>
#include <iostream>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>

#define MAX_PAYLOAD_BYTES 508

namespace bt {
    struct Socket {
    public:
        int const port;

        explicit Socket (int port);
        ~Socket();

        [[noreturn]] virtual void service () final;

        virtual void send (int receiver, std::string const & msg);

    protected:
        virtual void process (int sender, std::string const & msg);

    private:
        struct sockaddr_in address = {0};
        int const socket_fd;
    };
}

#endif //BACHELOR_SOCKET_H

/* Copyright © 2022 Aaron Alef */
