#include "Socket.h"

namespace bt {
    Socket::Socket (int port)
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

                LOG (INFO) << "Socket listening on port " << port;
            }

    Socket::~Socket () {
        close (socket_fd);
    }

    void Socket::service () {
        char buffer [MAX_PAYLOAD_BYTES] = {0};
        struct sockaddr_in sender = {0};

        while (true) {
            socklen_t length;
            std::size_t read = recvfrom (socket_fd, buffer, MAX_PAYLOAD_BYTES, MSG_WAITFORONE, (struct sockaddr *) & sender, & length);
            process (ntohs (sender.sin_port), {buffer, read});
        }
    }

    void Socket::send (int receiver, std::string const & msg) {
        LOG (INFO) << " SEND | " << receiver << " | " << msg.length() << "\t| " << msg;
        struct sockaddr_in recv_addr = { .sin_family = AF_INET
                                       , .sin_port = htons (receiver)
                                       , .sin_addr = {.s_addr = INADDR_ANY}};

        sendto (socket_fd, (char const *) msg.c_str(), msg.length(), 0, (struct sockaddr *) & recv_addr, sizeof (recv_addr));
    }

    void Socket::process (int sender, std::string const & msg) {
        LOG (INFO) << " RECV | " << sender << " | " << msg.length() << "\t| " << msg;
    }
}

/* Copyright © 2022 Aaron Alef */
