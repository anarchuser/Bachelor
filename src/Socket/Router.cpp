#include "Router.h"

namespace bt {
    Router::Router (port_t port, int timeout_ms)
            : port {port}
            , address { .sin_family = AF_INET
                      , .sin_port = htons (port)
                      , .sin_addr = {.s_addr = INADDR_ANY}
                      }
            , socket_fd {[&] () {
                auto fd = socket (AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    LOG (ERROR) << PRINT_PORT << "Could not create socket";
                }
                if (bind (fd, (struct sockaddr const *) & address, sizeof (address))) {
                    LOG (ERROR) << PRINT_PORT << "Could not bind to address";
                }
                return fd;
            }()}
            , timeout (std::chrono::milliseconds (timeout_ms))
            , thread {& Router::service, this}
            {
                char hostname[HOST_NAME_MAX];
                gethostname (hostname, sizeof (hostname));
                auto router_host = gethostbyname (hostname);
                auto const * addr_cp = (in_addr_t const *) router_host->h_addr_list[0];
                LOG (INFO) << "Router: " << addr2str (* addr_cp, port);
            }

    Router::~Router() {
        if (!should_stop) {
            LOG_IF (INFO, kLogCDtor) << PRINT_PORT << "[DTOR]";
            should_stop = true;
            thread.join ();
            close (socket_fd);
        }
    }

    void Router::service () {
        LOG_IF (INFO, kLogCDtor) << PRINT_PORT << "[CTOR]";

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
                    case EWOULDBLOCK: // No message available, wait
                        std::this_thread::yield();
                        continue;
                    default:
                        LOG (WARNING) << PRINT_PORT << "Unknown error: " << errno;
                }
            }

            buffer [read] = 0;
            auto const & packet = bt::Packet::from_buffer (buffer);
            if (ntohs (sender.sin_port) != packet.sender) {
                LOG (WARNING) << PRINT_PORT << "Received packet from port " << sender.sin_port << " with alleged sender " << packet.sender;
                LOG (WARNING) << PRINT_PORT << "Packet: " << packet;
            }
            send (packet, sender.sin_addr.s_addr);
            timeout.refresh();
        } while (! (should_stop && timeout.is_expired()));
    }

    void Router::send (Packet const & packet, in_addr_t receiver_address) const {
        LOG_IF (INFO, kLogRoute) << PRINT_PORT << "[ROUT|" << packet.sender << "->" << packet.receiver << "]\t[" << packet << "]";
        struct sockaddr_in recv_addr = { .sin_family = AF_INET
                , .sin_port = htons (packet.receiver)
                , .sin_addr = {.s_addr = receiver_address}};

        sendto (socket_fd, packet.c_str(), packet.size, 0, (struct sockaddr *) & recv_addr, sizeof (recv_addr));
    }
}

/* Copyright © 2022 Aaron Alef */
