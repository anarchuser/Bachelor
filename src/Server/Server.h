#ifndef BACHELOR_SERVER_H
#define BACHELOR_SERVER_H

#include <glog/logging.h>
#include <iostream>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>

#define MAX_PAYLOAD_BYTES 508

namespace bt {
    struct Server {
    public:
        int const port;

        explicit Server (int port);
        ~Server();

        [[noreturn]] virtual void run () final;

    protected:
        virtual void process (std::string const & packet);

    private:
        struct sockaddr_in address = {0};
        int const socket_fd;
    };
}

#endif //BACHELOR_SERVER_H

/* Copyright © 2022 Aaron Alef */
