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

#include <glog/logging.h>
#include <memory>

#include "config.h"
#include "Socket/Socket.h"
#include "Socket/Peer.h"
#include "Socket/Router.h"

#define PORT(n) (PORT_PEER_START + n)
#define PORT_INC port_index++

#define TIMEOUT_MS 4000

#define PEERS 5

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    bt::Router r (PORT_ROUTER, TIMEOUT_MS);
    bt::Socket::router = PORT_ROUTER;

    std::vector <std::unique_ptr <bt::Peer>> peers;
    for (int i = 0; i < PEERS; i++) {
        peers.push_back (std::make_unique <bt::Peer> (PORT(i)));
    }
    for (int i = 1; i < PEERS; i++) {
        peers[i]->join (PORT(i - 1));
    }

    std::this_thread::sleep_for (std::chrono::seconds (1));
    LOG (INFO) << "Who knows whom?";
    LOG (INFO) << "--------------------------------\\";
    for (int i = 0; i < PEERS; i++) {
        peers[i]->operator << (LOG (INFO));
    }
    LOG (INFO) << "--------------------------------/";
}
