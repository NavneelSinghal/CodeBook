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
