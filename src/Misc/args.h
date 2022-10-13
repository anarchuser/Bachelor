#ifndef BACHELOR_ARGS_H
#define BACHELOR_ARGS_H

#include <iostream>
#include <cstdlib>
#include <cstring>

// Program argument flags:
#define ARGS_HELP       'h', "--help"
#define ARGS_PEERS      'p', "--peers"
#define ARGS_ROUTER     'r', "--router"
#define ARGS_ADDRESS    'a', "--address"
#define ARGS_EXTERNAL   'e', "--external"
#define ARGS_STATE      's', "--state"
#define ARGS_MESSAGES   'm', "--msgs"
#define ARGS_TRUST      't', "--trust"

// Scenario commands
#define ARGS_DURATION   'd', "--duration"  // in seconds
#define ARGS_FREQUENCY  'f', "--frequency" // in actions / second

struct Args {
    enum Trust {
        NAIVE = 0,
        VOTING,
        LOCKSTEP,
    };

    Args (int argc, char const * * argv);

    int getIndex (char flag, char const * name) const;

    void isHelp() const;
    [[nodiscard]] inline int    getPeers() const { return getTypeInt (ARGS_PEERS); }
    [[nodiscard]] bool          getRouter() const;
    [[nodiscard]] char const *  getAddress() const;
    [[nodiscard]] bool          getExternal() const;
    [[nodiscard]] inline int    getState() const { return getTypeInt (ARGS_STATE); }
    [[nodiscard]] inline int    getMessageCount() const { return getTypeInt (ARGS_MESSAGES); }
    [[nodiscard]] Trust         getTrust() const;

    [[nodiscard]] inline int    getDuration() const { return getTypeInt (ARGS_DURATION); }
    [[nodiscard]] inline int    getFrequency() const { return getTypeInt (ARGS_FREQUENCY); }

private:
    [[nodiscard]] int  getTypeInt  (char flag, char const * name) const;

    int const argc;
    char const * * argv;
};

#endif //BACHELOR_ARGS_H

/* Copyright © 2022 Aaron Alef */
