#include "args.h"

Args::Args (int argc, char const * * argv): argc {argc}, argv {argv} {}

int Args::getIndex (char const * short_form, char const * long_form) const {
    for (int i = 1; i < argc; i++) {
        if (!strcmp (short_form, argv[i]) || !strcmp (long_form, argv[i])) {
            return i;
        }
    }
    return 0;
}

void Args::isHelp() const {
    static bool firstInvoc = true;
    if (!firstInvoc || !getIndex ("-h", "--help")) return;

    std::cout << "Program arguments:\n";
    std::cout << "  -h --help                 Display this as help\n";
    std::cout << "  -p --peers [n]            Number of peers\n";
    std::cout << "  -r --router               Set up a router\n";
    std::cout << "  -a --address              Use this address for router\n";
    std::cout << "  -s --state [n]            Initial state\n";
    std::cout << "  -m --msgs [n]             Number of messages to be sent\n";
    std::cout << "  -t --trust [naive|voting] Select the behaviour of peers\n" << std::endl;

    firstInvoc = false;
}

int Args::getPeers() const {
    return 0;
}

bool Args::getRouter() const {
    return false;
}

char const * Args::getAddress() const {
    return "localhost";
}

int Args::getState() const {
    return 0;
}

int Args::getMessageCount() const {
    return 0;
}

Args::Trust Args::getTrust() const {
    return NAIVE;
}

/* Copyright © 2022 Aaron Alef */
