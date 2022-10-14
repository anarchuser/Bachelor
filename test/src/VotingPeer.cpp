#include <catch2/catch_test_macros.hpp>

#include "config.h"

#include "Socket/log.h"
#include "Socket/Router.h"
#include "Socket/Peer.h"
#include "Socket/VotingPeer.h"
#include "Random/RNG.h"

#define PORT(n) (PORT_PEER_START + n)
#define PEERS 10
#define ROUTER

#define TIMEOUT_MS 400
#define IDLE_MS    100

#define INIT_STATE 00
#define MSG_NUM 1000
#define MSG_DELAY_MS 5

SCENARIO ("Random packets between voting peers get synchronised perfectly") {
    GIVEN ("Peers connected to a network") {
        kLogPeerDtorState = false;

        RNG rng;
        std::vector <std::unique_ptr <bt::Peer>> peers;
        for (int i = 0; i < PEERS; i++) {
            peers.push_back (std::make_unique <bt::VotingPeer> (PORT(i), INIT_STATE, TIMEOUT_MS));
        }
        REQUIRE_FALSE (peers.empty());
        for (int i = 1; i < PEERS; i++) {
            peers[i]->connect (PORT(i - 1));
        }
        while (std::any_of (peers.begin (), peers.end (), [] (auto const & peer) {
            return peer->num_of_peers < PEERS - 1;
        })) std::this_thread::sleep_for (std::chrono::milliseconds (PEERS * PEERS));

        WHEN ("Random ADD actions are performed (if allowed)") {
            for (int i = 0; i < MSG_NUM; i++) {
                peers[rng.random ({0, PEERS})]->act (rng.random ({-5, 5}));
                std::this_thread::sleep_for (std::chrono::milliseconds (MSG_DELAY_MS));
            }

            THEN ("The resulting states are perfectly synchronous") {
                std::vector <bt::IntState> result;
                for (auto const & peer : peers) {
                    auto state = peer->getState();
                    for (auto const & other : result) REQUIRE (state == other);
                    result.emplace_back (std::move (state));
                }
            }
            THEN ("All actions are permitted") {
                for (auto const & peer : peers) {
                    auto const & finalState = peer->getState();
                    auto state = finalState.initialState;
                    for (auto action : finalState.getActions()) {
                        REQUIRE (action.first.what != bt::FORBIDDEN);
                        state += action.first.value.change;
                        REQUIRE (state >= 0);
                    }
                }
            }
        }
        WHEN ("Random MOVE actions are performed") {
            for (int i = 0; i < MSG_NUM; i++) {
                int index = std::floor (rng.random (Bounds (0, PEERS)));
                auto & peer = * peers[index];
                bt::Position change (rng.random (Bounds (-5, 5)), rng.random (Bounds (-5, 5)));
                peer.move (change);
                std::this_thread::sleep_for (std::chrono::milliseconds (MSG_DELAY_MS));
            }
            std::this_thread::sleep_for (std::chrono::milliseconds (500));

            std::unordered_map <bt::port_t, bt::PosState> positions;
            for (auto const & other : peers) {
                auto first_pos = peers.front()->getState(other->port);
                positions.emplace (other->port, std::move (first_pos));
            }

            THEN ("The resulting states are perfectly synchronous") {
                for (auto const & peer : peers) {
                    for (auto const & other : peers) {
                        auto state = peer->getState(other->port);
                        REQUIRE (state == positions.at (other->port));
                    }
                }
            }
            THEN ("All actions are permitted") {
                for (auto const & peer : peers) {
                    auto const & finalState = peer->getState();
                    auto state = finalState.initialState;
                    for (auto action : finalState.getActions()) {
                        REQUIRE (action.first.what != bt::FORBIDDEN);
                        state += action.first.value.change;
                        REQUIRE (state >= 0);
                    }
                }
            }
        }
    }
}

/* Copyright © 2022 Aaron Alef */