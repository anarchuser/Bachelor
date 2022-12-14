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
            , send_fd {[&] () {
                auto fd = socket (AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    LOG (ERROR) << "\t" << PORT_ROUTER_OUT << ": Could not create socket";
                }
                if (bind (fd, (struct sockaddr const *) & address_out, sizeof (address_out))) {
                    LOG (ERROR) << "\t" << PORT_ROUTER_OUT << ": Could not bind to address";
                }
                return fd;
            }()}
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
            {
                static bool first_packet = true;
                LOG_IF (INFO, first_packet) << PRINT_PORT << "Connection established to " << addr2str (sender.sin_addr.s_addr);
                first_packet = false;
            }
            std::string packet_copy (buffer, read);
            if (ntohs (sender.sin_port) != packet.sender) {
                LOG (WARNING) << PRINT_PORT << "Received packet from port " << sender.sin_port << " with alleged sender " << packet.sender;
                LOG (WARNING) << PRINT_PORT << "Packet: " << to_string (packet);
            }
            {
                std::lock_guard guard (mx);
                auto latency = get_latency (packet.receiver, packet.sender);
                queue.push (
                        { std::chrono::steady_clock::now() + latency
                        , std::move (packet_copy)
                        , sender.sin_addr.s_addr
                        });
                queue_empty = false;
            }
            checkpoint.refresh();
        } while (!should_stop || !checkpoint.has_elapsed (timeout));
        receive_stop = true;
    }

    void Router::process () {
        while (!receive_stop) {
            if (queue_empty) continue;
            auto [time, packet, recv_addr] = [&]() {
                std::lock_guard guard (mx);
                auto item = queue.top();
                queue.pop();
                queue_empty = queue.empty();
                return item;
            }();
            std::this_thread::sleep_until (time);
            send (Packet::from_buffer (packet.c_str()), recv_addr);
        }
        while (!queue.empty()) {
            auto [time, packet, recv_addr] = queue.top();
            queue.pop ();
            std::this_thread::sleep_until (time);
            send (Packet::from_buffer (packet.c_str()), recv_addr);
        }
    }

    void Router::send (Packet const & packet, in_addr_t receiver_address) const {
        LOG_IF (INFO, kLogRoute) << "\t" << PORT_ROUTER_OUT << ": [ROUT|" << packet.sender << "->" << packet.receiver << "]\t[" << to_string (packet) << "]";
        struct sockaddr_in recv_addr = { .sin_family = AF_INET
                , .sin_port = htons (packet.receiver)
                , .sin_addr = {.s_addr = receiver_address}};

        sendto (send_fd, packet.c_str(), packet.size, 0, (struct sockaddr *) & recv_addr, sizeof (recv_addr));
    }

     constexpr std::chrono::milliseconds get_latency (int a, int b) {
        double x = a - PORT_PEER_START + b - PORT_PEER_START;
        double latency = ROUTER_LATENCY;
#ifdef LINEAR
        latency = ROUTER_DEV * ROUTER_LATENCY * x / ROUTER_PEERS + (1.0 - ROUTER_DEV) * ROUTER_LATENCY;
#endif
#ifdef QUINTIC
        latency = ROUTER_DEV * ROUTER_LATENCY * std::pow (0.5 * ROUTER_PEERS, -5) * std::pow (0.5 * (x - ROUTER_PEERS), 5) + ROUTER_LATENCY;
#endif
        return std::chrono::milliseconds {int (std::round (latency))};
    }
}

/* Copyright ?? 2022 Aaron Alef */
