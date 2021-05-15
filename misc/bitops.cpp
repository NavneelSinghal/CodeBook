__attribute__((target("popcnt"))) int popcnt_ll(uint64_t x) {
    return __builtin_popcountll(x);
}

__attribute__((target("popcnt"))) int popcnt(uint32_t x) {
    return __builtin_popcount(x);
}

// use only if popcnt is not available
int popcnt_own(uint64_t x) {
    x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x & 0x0F0F0F0F0F0F0F0FULL) + ((x >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    return (x * 0x0101010101010101ULL) >> 56;
}

int ctz_ll(uint64_t x) {
    return __builtin_ctzll(x);
}

int ctz(uint32_t x) {
    return __builtin_ctz(x);
}

// use only if __builtin_ctz not available
int ctz_hack_ll(uint64_t x) {
    if (__builtin_expect(x == 0, 0)) return 64;
    double f = (double)(x & -x);
    return (*(uint64_t *)&f >> 52) - 1023;
}

int ctz_hack(uint32_t x) {
    if (__builtin_expect(x == 0, 0)) return 32;
    float f = (float)(x & -x);
    return (*(uint32_t *)&f >> 23) - 127;
}

int ctz_lookup_ll(uint64_t x) {
    static constexpr int lookup[67] = {
        64, 66, 1,  39, 2,  15, 40, 23, 3,  12, 16, 59, 41, 19, 24, 54, 4,
        64, 13, 10, 17, 62, 60, 28, 42, 30, 20, 51, 25, 44, 55, 47, 5,  32,
        65, 38, 14, 22, 11, 58, 18, 53, 63, 9,  61, 27, 29, 50, 43, 46, 31,
        37, 21, 57, 52, 8,  26, 49, 45, 36, 56, 7,  48, 35, 6,  34, 33};
    return lookup[(x & -x) % 67];
}

int ctz_lookup(uint32_t x) {
    static constexpr int lookup[37] = {32, 36, 1,  26, 2,  23, 27, 32, 3,  16,
                                       24, 30, 28, 11, 33, 13, 4,  7,  17, 35,
                                       25, 22, 31, 15, 29, 10, 12, 6,  34, 21,
                                       14, 9,  5,  20, 8,  19, 18};
    return lookup[(x & -x) % 37];
}
