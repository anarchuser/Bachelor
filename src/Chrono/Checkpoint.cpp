#include "Checkpoint.h"

namespace bt {
    Checkpoint::Checkpoint (): checkpoint {high_resolution_clock::now()} {}

    void Checkpoint::refresh() {
        checkpoint = high_resolution_clock::now();
    }
    bool Checkpoint::has_elapsed (milliseconds duration) const {
        if (duration < milliseconds::zero()) return false;

        auto now = high_resolution_clock::now();
        auto elapsed_time = now - checkpoint.load();
        return elapsed_time > duration;
    }
}

/* Copyright © 2022 Aaron Alef */
