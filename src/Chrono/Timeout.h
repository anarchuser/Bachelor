#ifndef BACHELOR_TIMEOUT_H
#define BACHELOR_TIMEOUT_H

#include <chrono>
#include <glog/logging.h>

namespace bt {
    using namespace std::chrono;

    /// Helper class to track timeouts
    class Timeout {
    public:
        explicit Timeout (milliseconds timeout);

        void refresh();
        [[nodiscard]] bool is_expired() const;

        milliseconds const timeout;

    private:
        time_point <high_resolution_clock> checkpoint;
    };
}

#endif //BACHELOR_TIMEOUT_H

/* Copyright © 2022 Aaron Alef */
