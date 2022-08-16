#include "Socket.h"

namespace bt {
    Socket::Socket (port_t port)
            : port {port}
            , address { .sin_family = AF_INET
                      , .sin_port = htons (port)
                      , .sin_addr = {.s_addr = INADDR_ANY}
                      }
            , socket_fd {[&] () {
                auto fd = socket (AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    throw std::domain_error ("Could not create socket");
                }
                if (bind (fd, (struct sockaddr const *) (& address), sizeof (address))) {
                    throw (std::domain_error ("Could not bind to address"));
                }
                return fd;
            }()}
            , thread {& Socket::service, this} {}

    Socket::~Socket () {
        should_stop = true;
        thread.join();
        close (socket_fd);
    }

    void Socket::service () {
        LOG (INFO) << "Socket listening on port " << port;

        char buffer [MAX_PAYLOAD_BYTES] = {0};
        struct sockaddr_in sender = {0};
        socklen_t length;

        do {
            std::size_t read = recvfrom ( socket_fd
                                        , buffer
                                        , MAX_PAYLOAD_BYTES - 1
                                        , MSG_DONTWAIT
                                        , (struct sockaddr *) & sender
                                        , & length
                                        );
            if (!length) {
                std::this_thread::yield();
                continue;
            }
            buffer [read] = 0;
            process (Packet::from_buffer (buffer), ntohs (sender.sin_port));
        } while (!should_stop);
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
