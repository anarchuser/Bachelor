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

#include <chrono>
#include <glog/logging.h>

#include "Socket/Router.h"

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    auto const kTimeout = argc > 1 ? std::stoi (argv[1]) : ROUTER_TIMEOUT_MS;
    auto const kPacketLoss = argc > 2 ? std::stoi (argv[2]) : 0;
    bt::Router (PORT_ROUTER, kTimeout, kPacketLoss);
}
