#ifndef BACHELOR_PACKET_H
#define BACHELOR_PACKET_H

namespace bt {
    struct Packet {
        int const receiver;
        int const sender;
        char const * const msg;

        explicit Packet (char const * msg);
        [[nodiscard]] char const * c_str() const;
    };
};

#endif //BACHELOR_PACKET_H

/* Copyright © 2022 Aaron Alef */
