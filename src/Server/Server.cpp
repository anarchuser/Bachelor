#include "Server.h"

namespace bt {
    Server::Server (int port)
            : port {port}
            , address { .sin_family = AF_INET
                      , .sin_port = htons (port)
                      , .sin_addr = {.s_addr = INADDR_ANY}
                      }
            , socket_fd {socket (AF_INET, SOCK_DGRAM, 0)}
            {
                if (socket_fd < 0) {
                    throw std::domain_error ("Could not create socket");
                }
                if (bind (socket_fd, (struct sockaddr const *) (& address), sizeof (address))) {
                    throw (std::domain_error ("Could not bind to address"));
                }

                LOG (INFO) << "Server listening on port " << port;
            }

    Server::~Server () {
        close (socket_fd);
    }

    void Server::run () {
        char buffer [MAX_PAYLOAD_BYTES] = {0};
        struct sockaddr_in client = {0};

        while (true) {
            socklen_t length;
            std::size_t read = recvfrom (socket_fd, buffer, MAX_PAYLOAD_BYTES, MSG_WAITFORONE, (struct sockaddr *) & client, & length);
            process ({buffer, read});
        }
    }

    void Server::process (std::string const & packet) {
        LOG (INFO) << packet.length() << "\t| " << packet;
    }
}

/* Copyright © 2022 Aaron Alef */
