/*
 * TODO:
 all the usual stuff +:
DONE - first set bit
DONE - next set bit
DONE - last set bit
DONE - first unset bit
DONE - next unset bit
DONE - last unset bit
- find kth set
- find kth unset
- splicing a range to use as a mask
*/

#pragma GCC optimize("O3")
// #pragma GCC target("avx,avx2,avx512bitalg,sse,sse2,sse3,sse4")

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>

template <uint32_t N>
struct Bitset {
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;
    using i32 = std::int32_t;

   private:
    static constexpr u64 ONE = 1;
    static constexpr u64 ZERO = 0;
    static constexpr u64 ALL_MASK = ~ZERO;
    static constexpr u32 BITS = sizeof(u64) * CHAR_BIT;
    static constexpr u32 ALL = BITS - 1;
    static constexpr u32 CAPACITY = (N + BITS - 1) / BITS;
    static constexpr u32 REMAINING = N & ALL;
    static constexpr u64 CLEANUP_MASK = ~(ALL_MASK << REMAINING);
    u64 a[CAPACITY];
    /**
     * storage:
     * a[0] = bit_63,   ..., bit_0
     * a[1] = bit_127,  ..., bit_64
     * ...
     *
     * semantics of <<= and >>= : usual <<= and >>= semantics for u64
     */

    // NOTE: range violations are undefined behaviour and are unchecked
    struct reference {
       private:
        u32 pos;
        u64* a;

       public:
        reference(u32 p, u64* aa) : pos(p), a(aa) {}
        reference& operator=(bool x) noexcept {
            if (x)
                *a |= ONE << pos;
            else
                *a &= ~(ONE << pos);
            return *this;
        }
        reference& operator=(const reference& x) noexcept {
            if ((*x.a >> x.pos) & 1)
                *a |= ONE << pos;
            else
                *a &= ~(ONE << pos);
            return *this;
        }
        operator bool() const noexcept { return (*a >> pos) & 1; }
        bool operator~() const noexcept { return !((*a >> pos) & 1); }
        reference& flip() noexcept { *a ^= ONE << pos; }
    };

    inline void cleanup() noexcept { a[CAPACITY - 1] &= CLEANUP_MASK; }

   public:
    constexpr Bitset() noexcept {
        static_assert(BITS == 64, "Won't work on this architecture");
        static_assert(N > 0, "Size of parameter should be a positive integer");
        reset();
    };

    constexpr Bitset(u64 x) noexcept : Bitset() { a[0] = x; };

    constexpr Bitset(const std::string& x) : Bitset() {
#ifdef DEBUG
        if (x.size() > N * BITS)
            throw std::out_of_range("String is too large to convert to Bitset");
#endif
        u32 cur_loc = 0;
        u32 bit = 0;
        for (const auto c : x) {
#ifdef DEBUG
            if (c != '0' && c != '1')
                throw std::invalid_argument(
                    "String should consist of only 0s and 1s");
#endif
            a[cur_loc] |= u64(c - '0') << (bit++);
            if (bit == BITS) bit = 0, ++cur_loc;
        }
    }

    constexpr Bitset(const Bitset& b) {
        std::copy(std::begin(b.a), std::end(b.a), std::begin(a));
    }

    bool operator==(const Bitset& b) const {
        return std::equal(std::begin(a), std::end(a), std::begin(b.a));
    }

    bool operator!=(const Bitset& b) const { return !(*this == b); }

    constexpr bool operator[](u32 pos) const {
#ifdef DEBUG
        assert(("Bitset index out of range, UB", pos >= 0 && pos < N));
#endif
        return (a[pos / BITS] >> (pos & ALL)) & 1;
    }

    reference operator[](u32 pos) {
#ifdef DEBUG
        assert(("Bitset index out of range, UB", pos >= 0 && pos < N));
#endif
        return reference(pos & ALL, a + pos / BITS);
    }

    std::string to_string() const {
        std::string s(N, '0');
        auto it = s.data() + N - 1;
        for (u32 i = 0; i < N; ++i) *(it--) += (a[i / BITS] >> (i & ALL)) & 1;
        return s;
    }

    u32 count() const noexcept {
        u32 ans = 0;
        for (u32 i = 0; i < CAPACITY; ++i) ans += __builtin_popcountll(a[i]);
        return ans;
    }

    u32 size() const noexcept { return N; }

#define IMPLEMENT(op)                                      \
    Bitset& operator op(const Bitset& b) noexcept {        \
        for (u32 i = 0; i < CAPACITY; ++i) a[i] op b.a[i]; \
        return *this;                                      \
    }
    IMPLEMENT(&=)
    IMPLEMENT(|=)
    IMPLEMENT(^=)
#undef IMPLEMENT
    Bitset& flip() noexcept {
        for (u32 i = 0; i < CAPACITY; ++i) a[i] = ~a[i];
        cleanup();
        return *this;
    }
    Bitset operator~() const noexcept { return Bitset(*this).flip(); }

    Bitset& operator<<=(u32 n) noexcept {
        if (n > N) {
            reset();
            return *this;
        }
        if (__builtin_expect(n != 0, 1)) {
            const u32 loc = n / BITS;
            const u32 pos = n & ALL;
            if (pos == 0)
                for (u32 i = CAPACITY - 1; i != loc - 1; --i) a[i] = a[i - loc];
            else {
                const u32 complement = BITS - pos;
                for (u32 i = CAPACITY - 1; i != loc - 1; --i)
                    a[i] = (a[i - loc] << pos) | (a[i - loc - 1] >> complement);
                a[loc] = a[0] << pos;
            }
            std::fill(std::begin(a), std::begin(a) + loc, 0);
            cleanup();
        }
        return *this;
    }

    // 01000 -> 00100
    Bitset& operator>>=(u32 n) noexcept {
        if (n > N) {
            reset();
            return *this;
        }
        if (__builtin_expect(n != 0, 1)) {
            const u32 loc = n / BITS;
            const u32 pos = n & ALL;
            const u32 l = CAPACITY - 1 - loc;
            if (pos == 0)
                for (u32 i = 0; i <= l; ++i) a[i] = a[i + loc];
            else {
                const u32 complement = BITS - pos;
                for (u32 i = 0; i < l; ++i)
                    a[i] = (a[i + loc] >> pos) | (a[i + loc + 1] << complement);
                a[l] = a[CAPACITY - 1] >> pos;
            }
            std::fill(std::begin(a) + l + 1, std::end(a), 0);
            cleanup();
        }
        return *this;
    }

    Bitset operator<<(u32 n) const noexcept { return Bitset(*this) <<= n; }
    Bitset operator>>(u32 n) const noexcept { return Bitset(*this) >>= n; }

    void reset() noexcept { std::fill(std::begin(a), std::end(a), 0); }

    void set() noexcept {
        std::fill(std::begin(a), std::end(a), ALL_MASK);
        cleanup();
    }

    // warning: these use ints and not u

    i32 find_first_set() const noexcept {
        for (i32 i = 0; i < CAPACITY; ++i) {
            u64 w = a[i];
            if (w) return i * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_first_unset() const noexcept {
        for (i32 i = 0; i < CAPACITY; ++i) {
            u64 w = ~a[i];
            if (w) return i * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_next_set(i32 i) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        i32 loc = i / BITS;
        u64 w = a[loc] & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        for (++loc; loc < CAPACITY; ++loc) {
            w = a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_next_unset(i32 i) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        i32 loc = i / BITS;
        u64 w = (~a[loc]) & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        for (++loc; loc < CAPACITY; ++loc) {
            w = ~a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_prev_set(i32 i) const noexcept {
        --i;
        if (i < 0) return -1;
        i32 loc = i / BITS;
        u64 w = a[loc] & (ALL_MASK >> (ALL ^ (i & ALL)));
        if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        for (--loc; loc >= 0; --loc) {
            w = a[loc];
            if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    i32 find_prev_unset(i32 i) const noexcept {
        --i;
        if (i < 0) return -1;
        i32 loc = i / BITS;
        u64 w = (~a[loc]) & (ALL_MASK >> (ALL ^ (i & ALL)));
        if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        for (--loc; loc >= 0; --loc) {
            w = ~a[loc];
            if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    i32 find_next_set(i32 i, i32 max_iter) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        i32 loc = i / BITS;
        u64 w = a[loc] & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        int iter = 0;
        for (++loc; loc < CAPACITY && iter < max_iter; ++loc, ++iter) {
            w = a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_next_unset(i32 i, i32 max_iter) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        i32 loc = i / BITS;
        u64 w = (~a[loc]) & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        int iter = 0;
        for (++loc; loc < CAPACITY && iter < max_iter; ++loc, ++iter) {
            w = ~a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    i32 find_prev_set(i32 i, i32 max_iter) const noexcept {
        --i;
        if (i < 0) return -1;
        i32 loc = i / BITS;
        u64 w = a[loc] & (ALL_MASK >> (ALL ^ (i & ALL)));
        if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        int iter = 0;
        for (--loc; loc >= 0 && iter < max_iter; --loc, ++iter) {
            w = a[loc];
            if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    i32 find_prev_unset(i32 i, i32 max_iter) const noexcept {
        --i;
        if (i < 0) return -1;
        i32 loc = i / BITS;
        u64 w = (~a[loc]) & (ALL_MASK >> (ALL ^ (i & ALL)));
        if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        int iter = 0;
        for (--loc; loc >= 0 && iter < max_iter; --loc, ++iter) {
            w = ~a[loc];
            if (w) return loc * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    i32 find_last_set() const noexcept {
        for (i32 i = CAPACITY - 1; i >= 0; --i) {
            u64 w = a[i];
            if (w) return i * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    i32 find_last_unset() const noexcept {
        for (i32 i = CAPACITY - 1; i >= 0; --i) {
            u64 w = ~a[i];
            if (w) return i * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    // i32 find_kth_set(i32 k) const noexcept {}

    // i32 find_kth_unset(i32 k) const noexcept {}

    // i32 find_kth_next_set(i32 i, i32 k) const noexcept {}

    // i32 find_kth_next_unset(i32 i, i32 k) const noexcept {}

    // i32 find_kth_last_set(i32 k) const noexcept {}

    // i32 find_kth_last_unset(i32 k) const noexcept {}

    // return [l, r)
    u64 slice(const u32 l, const u32 r) const {
#ifdef DEBUG
        assert(l <= N);
        assert(r <= N);
#endif
        if (l >= r) return 0;
        // 0 <= l < r <= N
        u32 i1 = l / BITS;
        u32 pos1 = l & ALL;
        u32 i2 = (r - 1) / BITS;
        u32 pos2 = (r - 1) & ALL;
        if (i1 == i2) {
            return (a[i1] & (ALL_MASK >> (ALL - pos2))) >> pos1;
        } else {
#ifdef DEBUG
            assert(i2 == i1 + 1);
#endif
            return (a[i1] >> pos1) |
                   ((a[i2] & (ALL_MASK >> (ALL - pos2))) << (BITS - pos1));
        }
    }

    std::string to_string_debug() const {
        std::string s(CAPACITY * BITS, '0');
        for (i32 i = 0; i < CAPACITY * BITS; ++i)
            s[CAPACITY * BITS - 1 - i] += (a[i / BITS] >> (i & ALL)) & 1;
        return s;
    }

    friend inline Bitset operator&(const Bitset& x, const Bitset& y) noexcept {
        Bitset result = x;
        result &= y;
        return result;
    }
    friend inline Bitset operator|(const Bitset& x, const Bitset& y) noexcept {
        Bitset result = x;
        result |= y;
        return result;
    }

    friend inline Bitset operator^(const Bitset& x, const Bitset& y) noexcept {
        Bitset result = x;
        result ^= y;
        return result;
    }
    friend std::ostream& operator<<(std::ostream& os, const Bitset& b) {
        return os << b.to_string();
    }
    friend std::istream& operator>>(std::istream& os, Bitset& b) {
        std::string s = "";
        os >> s;
        b = Bitset(s);
        return os;
    }
};

constexpr int BIG = 100;

int main() {
    Bitset<BIG> a(1);
    Bitset<BIG> b;
    // assert(a == b);
    std::cout << a << '\n';
    a[0] = 1;
    std::cout << a << '\n';
    b[1] = 1;
    std::cout << b << '\n';
    b[0] = a[0];
    std::cout << b << '\n';
    a ^= b;
    std::cout << a << '\n';
    Bitset<BIG> aa, ab;
    // aa[20] = 1;
    assert(aa == ab);
    aa[19] = 1;
    assert(aa != ab);
    std::cout << aa.count() << '\n';
    std::cout << aa << '\n';
    std::cout << ab << '\n';
    aa >>= 2;
    std::cout << aa << '\n';
    a <<= 10;
    std::cout << a << '\n';
    // aa <<= 19;
    // std::cout << aa << '\n';
    aa >>= 19;
    std::cout << aa << '\n';
    assert(aa == ab);
    ab[18] = 1;
    ab[1] = 1;
    ab[2] = 1;
    ab[5] = 1;
    ab &= ~aa;
    aa[2] = 1;
    std::cout << (~aa) << '\n';
    std::cout << ab << '\n';
    std::cout << ((~aa) & ab) << '\n';
    aa.set();
    std::cout << aa << '\n';
    aa.reset();
    std::cout << aa << '\n';
    aa.set();
    std::cout << aa << '\n';
    aa.reset();
    std::cout << aa << '\n';
    return 0;
}
