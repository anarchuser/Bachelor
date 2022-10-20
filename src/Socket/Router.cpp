#include "Router.h"

namespace bt {
    Router::Router (port_t port, timestamp_t timeout_ms)
            : port {port}
            , address_in { .sin_family = AF_INET
                         , .sin_port = htons (port)
                         , .sin_addr = {.s_addr = INADDR_ANY}
            }
            , address_out { .sin_family = AF_INET
                          , .sin_port = htons (PORT_ROUTER_OUT)
                          , .sin_addr = {.s_addr = INADDR_ANY}
            }
            , recv_fd {[&] () {
                auto fd = socket (AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    LOG (ERROR) << PRINT_PORT << "Could not create socket";
                }
                if (bind (fd, (struct sockaddr const *) & address_in, sizeof (address_in))) {
                    LOG (ERROR) << PRINT_PORT << "Could not bind to address";
                }
                return fd;
            }()}
            , send_fd {[&] () {
                auto fd = socket (AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    LOG (ERROR) << PRINT_PORT << "Could not create socket";
                }
                if (bind (fd, (struct sockaddr const *) & address_out, sizeof (address_out))) {
                    LOG (ERROR) << PRINT_PORT << "Could not bind to address";
                }
                return fd;
            }()}
            , timeout (std::chrono::milliseconds (timeout_ms))
            , receiver {& Router::service, this}
            , sender {& Router::process, this}
            {
                char hostname[HOST_NAME_MAX];
                gethostname (hostname, sizeof (hostname));
                auto router_host = gethostbyname (hostname);
                auto const * addr_cp = (in_addr_t const *) router_host->h_addr_list[0];
                LOG (INFO) << "\tRouter: " << addr2str (* addr_cp, port);
            }

    Router::~Router() {
        if (!should_stop.exchange (true)) {
            LOG_IF (INFO, kLogCDtor) << PRINT_PORT << "[DTOR]";
            receiver.join ();
            sender.join();
            close (recv_fd);
            close (send_fd);
        }
    }

    void Router::await_idle (timestamp_t idle) const {
        if (should_stop) return;
        while (!checkpoint.has_elapsed (std::chrono::milliseconds (idle))) std::this_thread::yield();
    }

    void Router::service () {
        LOG_IF (INFO, kLogCDtor) << PRINT_PORT << "[CTOR]";

        char buffer [MAX_PAYLOAD_BYTES] = {0};
        struct sockaddr_in sender = {0};
        socklen_t length = sizeof (struct sockaddr_in);
        std::this_thread::sleep_for (std::chrono::milliseconds (50));

        do {
            ssize_t read = recvfrom ( recv_fd
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
            std::string packet_copy (buffer, read);
            if (ntohs (sender.sin_port) != packet.sender) {
                LOG (WARNING) << PRINT_PORT << "Received packet from port " << sender.sin_port << " with alleged sender " << packet.sender;
                LOG (WARNING) << PRINT_PORT << "Packet: " << to_string (packet);
            }
            {
                std::lock_guard guard (mx);
                queue.push ({std::chrono::steady_clock::now(), std::move (packet_copy), sender.sin_addr.s_addr});
                queue_empty = false;
            }
            checkpoint.refresh();
        } while (!should_stop || !checkpoint.has_elapsed (timeout));
        receive_stop = true;
    }

    void Router::process () {
        std::chrono::milliseconds latency (ROUTER_LATENCY);
        while (!receive_stop) {
            if (queue_empty) continue;
            auto [time, packet, recv_addr] = [&]() {
                std::lock_guard guard (mx);
                auto item = queue.front();
                queue.pop();
                queue_empty = queue.empty();
                return item;
            }();
            std::this_thread::sleep_until (time + latency);
            send (Packet::from_buffer (packet.c_str()), recv_addr);
        }
        while (!queue.empty()) {
            auto [time, packet, recv_addr] = queue.front ();
            queue.pop ();
            std::this_thread::sleep_until (time + latency);
            send (Packet::from_buffer (packet.c_str()), recv_addr);
        }
    }

    void Router::send (Packet const & packet, in_addr_t receiver_address) const {
        LOG_IF (INFO, kLogRoute) << PRINT_PORT << "[ROUT|" << packet.sender << "->" << packet.receiver << "]\t[" << to_string (packet) << "]";
        struct sockaddr_in recv_addr = { .sin_family = AF_INET
                , .sin_port = htons (packet.receiver)
                , .sin_addr = {.s_addr = receiver_address}};

        sendto (send_fd, packet.c_str(), packet.size, 0, (struct sockaddr *) & recv_addr, sizeof (recv_addr));
    }
}

/* Copyright © 2022 Aaron Alef */
