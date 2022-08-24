#include <catch2/catch_test_macros.hpp>

#include "config.h"

#include "Socket/Socket.h"
#include "Packet/PingPacket.h"

#define PORT PORT_PEER_START

TEST_CASE ("Sockets can be created and destroyed") {
    REQUIRE_NOTHROW (bt::Socket (PORT_PEER_START));
}

TEST_CASE ("Sockets can send packets to itself") {
    bt::Socket s (PORT, 500);
    REQUIRE_NOTHROW (s.send (bt::PingPacket (s.port, s.port, 0)));
}
