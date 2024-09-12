#pragma once
#include <cstdint>


namespace Utils
{
    inline uint64_t BitShift(uint64_t x, int shift)
    {
        return ((shift > 0) ? (x << shift) : (x >> -shift));
    }


    inline int BitCounter(uint64_t x)
    {
        int bitCount = 0;
        while (x) {
            bitCount += (x & 0x1);
            x >>= 1;
        }
        return bitCount;
    }


    /*
        Bit bitmasks

            BitMaskA           BitMaskB
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B

            BitMaskA2          BitMaskB2
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
    */

    constexpr uint64_t BitMaskA = ~0x8080808080808080ULL;
    constexpr uint64_t BitMaskA2 = ~0xc0c0c0c0c0c0c0c0ULL;

    constexpr uint64_t BitMaskB = ~0x101010101010101ULL;
    constexpr uint64_t BitMaskB2 = ~0x303030303030303ULL;
}
