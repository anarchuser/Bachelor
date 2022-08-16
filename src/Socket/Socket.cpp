#include "Socket.h"

namespace bt {
    Socket::Socket (port_t port)
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
        socklen_t length;

        while (true) {
            std::size_t read = recvfrom ( socket_fd
                                        , buffer
                                        , MAX_PAYLOAD_BYTES - 1
                                        , MSG_WAITFORONE
                                        , (struct sockaddr *) & sender
                                        , & length
                                        );
            buffer [read] = 0;
            process (Packet::from_buffer (buffer), ntohs (sender.sin_port));
        }
    }

    void Socket::send (Packet const & packet) {
        send (packet, packet.header.receiver);
    }
    void Socket::send (Packet const & packet, port_t receiver) {
        LOG (INFO) << '\t' << port << ": [SEND|" << packet;
        struct sockaddr_in recv_addr = { .sin_family = AF_INET
                                       , .sin_port = htons (receiver)
                                       , .sin_addr = {.s_addr = INADDR_ANY}};

        sendto (socket_fd, packet.c_str(), packet.header.size, 0, (struct sockaddr *) & recv_addr, sizeof (recv_addr));
    }

    void Socket::process (Packet const & packet, port_t sender) {
        LOG (INFO) << '\t' << port << ": [RECV|" << packet;
    }
}

/* Copyright © 2022 Aaron Alef */
