#ifndef BACHELOR_ARGS_H
#define BACHELOR_ARGS_H

#include <iostream>
#include <cstdlib>
#include <cstring>

// Program argument flags:
#define ARGS_HELP       "-h", "--help"
#define ARGS_PEERS      "-p", "--peers"
#define ARGS_ROUTER     "-r", "--router"
#define ARGS_ADDRESS    "-a", "--address"
#define ARGS_STATE      "-s", "--state"
#define ARGS_MESSAGES   "-m", "--msgs"
#define ARGS_TRUST      "-t", "--trust"

struct Args {
    enum Trust {
        NAIVE = 0,
        VOTING,
        LOCKSTEP,
    };

    Args (int argc, char const * * argv);

    int getIndex (char const * short_form, char const * long_form) const;

    void isHelp() const;
    [[nodiscard]] int          getPeers() const;
    [[nodiscard]] bool         getRouter() const;
    [[nodiscard]] char const * getAddress() const;
    [[nodiscard]] int          getState() const;
    [[nodiscard]] int          getMessageCount() const;
    [[nodiscard]] Trust        getTrust() const;

private:
    int const argc;
    char const * * argv;
};

#endif //BACHELOR_ARGS_H

/* Copyright © 2022 Aaron Alef */
