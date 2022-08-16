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

#include "Socket/Socket.h"

#define PORT1 50000
#define PORT2 50001

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    bt::Socket s1 (PORT1);
    bt::Socket s2 (PORT2);

    std::thread thread (& bt::Socket::service, s1);

    s2.send ({PORT1, PORT2, "Hello, World!"});
    s2.send ({PORT1, PORT2, "0-,-1-,-2-,-3-,-4-,-5,-,6-,-7-,-0-,-1-,-2-,-3-,-4-,-5-,-6-,-7-,-"});
    for (char c = 'a'; c <= 'a'; c++) {
        s2.send ({PORT1, PORT2, std::string ("Counter = ") += c});
    }

    thread.join();
}
