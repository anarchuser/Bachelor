#include "args.h"

Args::Args (int argc, char const * * argv): argc {argc}, argv {argv} {}

int Args::getIndex (char flag, char const * name) const {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && strchr (argv[i] + 1, flag)) return i;
        if (!strcmp (argv[i], name)) return i;
    }
    return 0;
}

void Args::isHelp() const {
    static bool firstInvoc = true;
    if (!firstInvoc || !getIndex (ARGS_HELP)) return;
    firstInvoc = false;

    std::cout << "Program arguments:\n";
    std::cout << "  -h --help                 Display this as help\n";
    std::cout << "  -p --peers [n]            Number of peers\n";
    std::cout << "  -r --router               Set up a router\n";
    std::cout << "  -a --address              Use this address for router\n";
    std::cout << "  -s --state [n]            Initial state\n";
    std::cout << "  -m --msgs [n]             Number of messages to be sent\n";
    std::cout << "  -t --trust [naive|voting] Select the behaviour of peers\n" << std::endl;
}

int Args::getPeers() const {
    auto flag_index = getIndex (ARGS_PEERS);
    auto val_index = flag_index + 1;
    if (!flag_index || val_index >= argc) return 0;
    try {
        return std::stoi (argv [val_index]);
    } catch (...) {}
    return 0;
}

bool Args::getRouter() const {
    return getIndex (ARGS_ROUTER);
}

char const * Args::getAddress() const {
    auto flag_index = getIndex (ARGS_ADDRESS);
    auto val_index = flag_index + 1;
    if (!flag_index || val_index >= argc) return "localhost";
    return argv [val_index];
}

int Args::getState() const {
    auto flag_index = getIndex (ARGS_STATE);
    auto val_index = flag_index + 1;
    if (!flag_index || val_index >= argc) return 0;
    try {
        return std::stoi (argv [val_index]);
    } catch (...) {}
    return 0;
}

int Args::getMessageCount() const {
    auto flag_index = getIndex (ARGS_MESSAGES);
    auto val_index = flag_index + 1;
    if (!flag_index || val_index >= argc) return 0;
    try {
        return std::stoi (argv [val_index]);
    } catch (...) {}
    return 0;
}

Args::Trust Args::getTrust() const {
    auto flag_index = getIndex (ARGS_TRUST);
    auto val_index = flag_index + 1;
    if (!flag_index || val_index >= argc) return NAIVE;
    if (!strcmp (argv[val_index], "naive")) return NAIVE;
    if (!strcmp (argv[val_index], "voting")) return VOTING;
    if (!strcmp (argv[val_index], "lockstep")) return LOCKSTEP;
    return NAIVE;
}

/* Copyright © 2022 Aaron Alef */
