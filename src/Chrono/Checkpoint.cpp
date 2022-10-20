#include "Checkpoint.h"

namespace bt {
    Checkpoint::Checkpoint (): checkpoint {clock::now()} {}

    void Checkpoint::refresh() {
        checkpoint = clock::now();
    }
    bool Checkpoint::has_elapsed (milliseconds duration) const {
        if (duration < milliseconds::zero()) return false;

        auto now = clock::now();
        auto elapsed_time = now - checkpoint.load();
        return elapsed_time > duration;
    }
}

/* Copyright © 2022 Aaron Alef */
