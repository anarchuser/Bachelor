/* Copyright (C) 2022 Aaron Alef <aaron.alef@code.berlin>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (e.g., ./LICENSE).
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <memory>
#include <vector>

#include <glog/logging.h>

#include "config.h"
#include "Misc/args.h"
#include "Chrono/util.h"
#include "Packet/helper.h"
#include "Socket/Socket.h"
#include "Socket/Peer.h"
#include "Socket/NaivePeer.h"
#include "Socket/VotingPeer.h"
#include "Socket/Router.h"
#include "State/State.h"

#include "Random/RNG.h"

#define PORT(n) (PORT_PEER_START + n)

#define TIMEOUT_MS 400
#define IDLE_MS    100

#define PEERS 10

#define ARGS_INIT_STATE 100
#define ARGS_MSG_NUM 1000
#define MSG_DELAY_MS 10

/* Program arguments:
 * -h --help                    Display this as help
 * -p --peers [n]               Number of peers
 * -r --router                  Set up a router
 * -a --address                 Use this address for router
 * -s --state [n]               Initial state
 * -m --msgs [n]                Number of messages to be sent
 * -t --trust [naive|voting]    Select the behaviour of peers
 */

void updatePositions (std::vector<std::unique_ptr<bt::Peer>> & peers, RNG & rng, int duration, int frequency);

int main (int argc, char * argv[]) {
    /* Init section */
    Args const args {argc, (char const * *) argv};
    google::InitGoogleLogging (argv[0]);
    RNG rng;
    std::unique_ptr <bt::Router> r;

    /* Read program args, display help if required */
    args.isHelp ();
    auto const kPeers = args.getPeers() ?: PEERS;
    auto const kRouter = args.getRouter();
    auto const kRouterAddress = args.getAddress() ?: "localhost";
    auto const kExternalRouter = args.getExternal();
    auto const kInitState = args.getState() ?: ARGS_INIT_STATE;
    auto const kMessageCount = args.getMessageCount() ?: ARGS_MSG_NUM;
    auto const kTrustProtocol = args.getTrust();

    auto const kScenarioDuration = args.getDuration();
    auto const kScenarioFrequency = args.getFrequency();

    std::cout << "Scenario duration (in s):  " << kScenarioDuration  << std::endl;
    std::cout << "Scenario frequency (in s): " << kScenarioFrequency << std::endl;

    LOG (INFO) << "\t" << bt::get_time_string() << " ns: start";

    /* Configure router if needed */
    if (kRouter) {
        auto router_host = gethostbyname (kRouterAddress);
        bt::Socket::router_address = router_host ? * (in_addr_t *) (router_host->h_addr_list[0]) : INADDR_ANY;
        bt::Socket::router_port = PORT_ROUTER;

        if (!kExternalRouter || strcmp (kRouterAddress, "localhost")) {
            r = std::make_unique <bt::Router> (PORT_ROUTER, TIMEOUT_MS);
        } else {
            LOG (INFO) << "\tRouter: " << bt::addr2str (bt::Socket::router_address, bt::Socket::router_port);
        }
    } else {
        LOG (INFO) << "\tNo router in use.";
    }

    LOG (INFO) << "\t" << bt::get_time_string() << " ns: connect";
    {
        /* Build network */
        std::vector<std::unique_ptr<bt::Peer>> peers;
        for (int i = 0; i < kPeers; i++) {
            switch (kTrustProtocol) {
                case Args::NAIVE:
                    peers.push_back (std::make_unique <bt::NaivePeer> (PORT(i), kInitState, TIMEOUT_MS));
                    break;
                case Args::VOTING:
                    peers.push_back (std::make_unique <bt::VotingPeer> (PORT(i), kInitState, TIMEOUT_MS));
                    break;
                case Args::LOCKSTEP:
                    break;
                default:;
            }
        }
        /* Do any scenario action */
        if (peers.empty()) {
            LOG (WARNING) << "No peer created! Note that Lockstep is not available (yet)";
            return EXIT_FAILURE;
        }

        for (int i = 1; i < peers.size(); i++) {
            peers[i]->connect (PORT(i - 1));
        }

        /* Wait for network */
        while (std::any_of (peers.begin (), peers.end (), [kPeers] (auto const & peer) {
            return peer->num_of_peers < kPeers - 1;
        })) std::this_thread::sleep_for (std::chrono::milliseconds (kPeers * kPeers));

        LOG (INFO) << "\t" << bt::get_time_string() << " ns: act";

        std::thread physim (updatePositions, std::ref (peers), std::ref (rng), kScenarioDuration, kScenarioFrequency);

//        peers[0]->act (bt::NOOP);
//        peers[0]->act (bt::FORBIDDEN);
//        peers[1]->act (-30);
//
//        for (int i = 0; i < kMessageCount; i++) {
//            int index = std::floor (rng.random (Bounds (0, kPeers)));
//            auto & peer = * peers[index];
//            bt::Position change (rng.random (Bounds (-5, 5)), rng.random (Bounds (-5, 5)));
//            auto pos = peer.getState(peer.port).getState();
//            peer.move ({change, pos + change});
//            std::this_thread::sleep_for (std::chrono::milliseconds (5));
//        }

        physim.join();
        LOG (INFO) << "\t" << bt::get_time_string() << " ns: destruct";

        /* Wait for sync to finish */
        if (r) r.reset ();
        else std::this_thread::sleep_for (std::chrono::seconds (2));
        std::this_thread::sleep_for (std::chrono::seconds (1));

        /* Print all states */
        std::cout << "Peers: |  ";
        for (auto const & peer : peers) std::cout << peer->port << "|  ";
        std::cout << "\nState: |";
        for (auto const & peer : peers) std::cout << std::setfill(' ') << std::setw (7) << peer->getState().getState() << "|";
        {
            auto & owner = peers.front();
            std::cout << "\n" << "Coord: |";
            for (auto const & peer: peers) {
                std::cout << std::setfill(' ') << std::setw (7) << owner->getState (peer->port) << "|";
            }
        }
        std::cout << std::endl;

        /* Check that all states are actually the same in the end */
        std::vector <bt::IntState> result;
        for (auto const & peer : peers) {
            auto state = peer->getState();
            for (auto const & other : result) {
                CHECK_EQ (state, other);
            }
            result.emplace_back (std::move (state));
        }

        std::unordered_map <bt::port_t, bt::PosState> positions;
        for (auto const & other : peers) {
            auto first_pos = peers.front()->getState(other->port);
            positions.emplace (other->port, std::move (first_pos));
        }
        for (auto const & peer : peers) {
            for (auto const & other : peers) {
                auto state = peer->getState(other->port);
                CHECK_EQ (state, positions.at (other->port));
            }
        }
    }
    LOG (INFO) << "\t" << bt::get_time_string() << " ns: end";
}

void updatePositions (std::vector<std::unique_ptr<bt::Peer>> & peers, RNG & rng, int duration, int frequency) {
    auto const kPeers = peers.size();
    std::chrono::microseconds const idle (long (std::floor (1000000.0 / frequency)));
    for (int i = 0; i < duration * frequency; i++) {
        for (auto const & peer : peers) {
            bt::Position move (rng.random ({-5, 5}), rng.random ({-5, 5}));
            peer->move ({move, move});
        }
        std::this_thread::sleep_for (idle);
    }
}
