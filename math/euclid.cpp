int gcd (int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// use only for non-negative u, v
int gcd(int u, int v) {
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
    shift = __builtin_ctz(u | v);
    u >>= __builtin_ctz(u);
    do {
        v >>= __builtin_ctz(v);
        if (u > v) {
            swap(u, v);
        }
        v -= u;
    } while (v);
    return u << shift;
}

// use only for non-negative u, v
long long gcd(long long u, long long v) {
    int shift;
    if (u == 0 || v == 0) return u + v;
    shift = __builtin_ctzll(u | v);
    u >>= __builtin_ctzll(u);
    do {
        v >>= __builtin_ctzll(v);
        if (u > v) {
            swap(u, v);
        }
        v -= u;
    } while (v);
    return u << shift;
}

// in header file numeric
#define gcd __gcd
