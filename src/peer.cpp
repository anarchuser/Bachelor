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

#include <thread>

#include "config.h"
#include "Socket/Socket.h"
#include "Socket/Router.h"

#define PORT(n) (PORT_PEER_START + n)

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    bt::Router r  (PORT_ROUTER);
    bt::Socket s1 (PORT(0));

    s1.send ({PORT(0), PORT(0), "Hello, World!"}, PORT_ROUTER);
    for (char c = 'a'; c <= 'z'; c++) {
        s1.send ({PORT(0), PORT(0), std::string ("Counter = ") += c}, PORT_ROUTER);
    }
}
