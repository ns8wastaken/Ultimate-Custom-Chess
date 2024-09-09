#pragma once
#include <cstdint>


namespace Utils
{
    inline uint64_t BitShift(uint64_t x, int shift)
    {
        return ((shift > 0) ? (x << shift) : (x >> -shift));
    };
}
