#include "Timeout.h"

namespace bt {
    Timeout::Timeout (milliseconds timeout)
            : checkpoint {high_resolution_clock::now()}
            , timeout {timeout}
            {}

    void Timeout::refresh() {
        checkpoint = high_resolution_clock::now();
    }
    bool Timeout::is_expired () const {
        if (timeout < milliseconds::zero()) return false;

        auto now = high_resolution_clock::now();
        auto elapsed_time = now - checkpoint;
        return elapsed_time > timeout;
    }
}

/* Copyright © 2022 Aaron Alef */
