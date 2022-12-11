#include "onip/utils/utils.hpp"

#include <stdio.h>

// Not going to pretend to understand what is going on here, got this solution from here:
// https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
static_assert((RAND_MAX & (RAND_MAX + 1u)) == 0, "RAND_MAX not a Mersenne number");

namespace onip {
    int Utils::rand_int32() {
        return rand();
    }

    uint32_t Utils::randUint32() {
        return static_cast<uint32_t>(rand());
    }

    uint64_t Utils::randUint64() {
        // Not going to pretend to understand what is going on here, got this solution from here:
        // https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
        uint64_t r = 0;
        for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
            r <<= RAND_MAX_WIDTH;
            r ^= (unsigned) rand();
        }
        return r;
    }
}