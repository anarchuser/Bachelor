#ifndef BACHELOR_CHECKPOINT_H
#define BACHELOR_CHECKPOINT_H

#include <chrono>
#include <atomic>
#include <glog/logging.h>
#include "util.h"

namespace bt {
    using namespace std::chrono;

    /// Helper class to track timeouts
    class Checkpoint {
    public:
        Checkpoint();

        void refresh();
        [[nodiscard]] bool has_elapsed (milliseconds duration) const;

    private:
        std::atomic <time_point <clock>> checkpoint;
    };
}

#endif //BACHELOR_CHECKPOINT_H

/* Copyright © 2022 Aaron Alef */
