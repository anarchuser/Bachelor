#include "Socket.h"

namespace bt {
    Socket::Socket (port_t port, int timeout)
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
            , thread {& Socket::service, this}
            , timeout (std::chrono::milliseconds (timeout))
            {}

    Socket::~Socket () {
        if (!should_stop) {
            should_stop = true;
            thread.join ();
            close (socket_fd);
            is_destroyed = true;
            LOG (INFO) << PRINT_PORT << "[DTOR]";
        }
    }

    void Socket::service () {
        LOG (INFO) << PRINT_PORT << "[CTOR]";

        char buffer [MAX_PAYLOAD_BYTES] = {0};
        struct sockaddr_in sender = {0};
        socklen_t length = sizeof (struct sockaddr_in);

        do {
            ssize_t read = recvfrom ( socket_fd
                                        , buffer
                                        , MAX_PAYLOAD_BYTES - 1
                                        , MSG_DONTWAIT
                                        , (struct sockaddr *) & sender
                                        , & length
                                        );
            if (read < 0) {
                switch (errno) {
#if EAGAIN != EWOULDBLOCK  // POSIX.1-2001 does not require these to have the same value
                    case EAGAIN:
#endif
                    case EWOULDBLOCK:
                        std::this_thread::yield();
                        continue;
                    default:
                        LOG (WARNING) << "Unknown error: " << errno;
                }
            }

            buffer [read] = 0;
            process (Packet::from_buffer (buffer), ntohs (sender.sin_port));
            timeout.refresh();
        } while (! (should_stop && timeout.is_expired()));
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
