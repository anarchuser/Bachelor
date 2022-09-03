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
#include <unordered_set>
#include <memory>
#include <vector>

#include <glog/logging.h>

#include "config.h"
#include "Chrono/util.h"
#include "Packet/helper.h"
#include "Socket/Socket.h"
#include "Socket/Peer.h"
#include "Socket/Router.h"
#include "State/State.h"

#include "Random/RNG.h"

#define PORT(n) (PORT_PEER_START + n)

#define TIMEOUT_MS 400
#define IDLE_MS    100

#define PEERS 10

#define ROUTER
#define ROUTER_REQUIRED

#define INIT_STATE 100
#define MSG_NUM 1000
#define MSG_DELAY_MS 10

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    int const kPeers = argc > 1 ? std::stoi (argv[1]) : PEERS;

    LOG (INFO) << "\t" << bt::get_time_string() << " ns: start";

#ifdef ROUTER
    char const * kRouterAddress = argc > 2 ? argv [2] : "localhost";
    auto router_host = gethostbyname (kRouterAddress);
    bt::Socket::router_address = router_host ? * (in_addr_t *) (router_host->h_addr_list[0]) : INADDR_ANY;
    bt::Socket::router_port = PORT_ROUTER;


#ifdef ROUTER_REQUIRED
    bt::Router r (PORT_ROUTER, TIMEOUT_MS);
#else
    LOG (INFO) << "\tRouter: " << bt::addr2str (bt::Socket::router_address, bt::Socket::router_port);
#endif
#else
    LOG (INFO) << "\tNo router in use.";
#endif

    LOG (INFO) << "\t" << bt::get_time_string() << " ns: connect";
    {
        std::vector<std::unique_ptr<bt::Peer>> peers;
        for (int i = 0; i < kPeers; i++) {
            peers.push_back (std::make_unique<bt::Peer> (PORT(i), INIT_STATE, TIMEOUT_MS));
        }
        for (int i = 1; i < kPeers; i++) {
            peers[i]->connect (PORT(i - 1));
        }

        while (std::any_of (peers.begin (), peers.end (), [kPeers] (auto const & peer) {
            return peer->num_of_peers < kPeers - 1;
        })) std::this_thread::sleep_for (std::chrono::milliseconds (kPeers * kPeers));

        LOG (INFO) << "\t" << bt::get_time_string() << " ns: act";

        RNG rng;
        for (int i = 0; i < MSG_NUM; i++) {
            peers [rng.random ({0, double (kPeers)})]->act (rng.random ({-5, 5}));
            std::this_thread::sleep_for (std::chrono::milliseconds (MSG_DELAY_MS));
        }

        LOG (INFO) << "\t" << bt::get_time_string() << " ns: destruct";

#ifdef ROUTER_REQUIRED
        r.await_idle (IDLE_MS);
#else
        std::this_thread::sleep_for (std::chrono::seconds (2));
#endif

        std::vector <bt::IntState> result;
        for (auto const & peer : peers) {
            auto state = peer->getState();
            for (auto const & other : result) {
                CHECK_EQ (state, other);
            }
            result.emplace_back (std::move (state));
        }
    }
    LOG (INFO) << "\t" << bt::get_time_string() << " ns: end";
}
