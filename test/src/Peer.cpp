#include <catch2/catch_test_macros.hpp>

#include "config.h"

#include "Socket/log.h"
#include "Socket/Router.h"
#include "Socket/Peer.h"
#include "Random/RNG.h"

#define PORT(n) (PORT_PEER_START + n)
#define PEERS 10
#define ROUTER

#define TIMEOUT_MS 400
#define IDLE_MS    100

#define INIT_STATE 100
#define MSG_NUM 1000
#define MSG_DELAY_MS 10

SCENARIO ("Random packets between peers get synchronised perfectly") {
    GIVEN ("Peers connected to a network") {
        kLogPeerDtorState = false;

        RNG rng;
        bt::Router r (PORT_ROUTER, TIMEOUT_MS);
        std::vector <std::unique_ptr <bt::Peer>> peers;
        for (int i = 0; i < PEERS; i++) {
            peers.push_back (std::make_unique<bt::Peer> (PORT(i), INIT_STATE, TIMEOUT_MS));
        }
        for (int i = 1; i < PEERS; i++) {
            peers[i]->connect (PORT(i - 1));
        }
        while (std::any_of (peers.begin (), peers.end (), [] (auto const & peer) {
            return peer->num_of_peers < PEERS - 1;
        })) std::this_thread::sleep_for (std::chrono::milliseconds (PEERS * PEERS));

        WHEN ("Random ADD actions are performed") {
            for (int i = 0; i < MSG_NUM; i++) {
                peers[rng.random ({0, PEERS})]->act (rng.random ({-5, 5}));
                std::this_thread::sleep_for (std::chrono::milliseconds (MSG_DELAY_MS));
            }

            THEN ("The resulting states are perfectly synchronous") {
                std::vector <bt::IntState> result;
                for (auto const & peer : peers) {
                    auto state = peer->getState();
                    for (auto const & other : result) CHECK (state == other);
                    result.emplace_back (std::move (state));
                }
            }
        }

    }
}

/* Copyright © 2022 Aaron Alef */
