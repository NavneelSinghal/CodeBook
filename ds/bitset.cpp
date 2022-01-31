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

template <int N>
struct Bitset {
   private:
    static constexpr uint64_t ONE = 1;
    static constexpr uint64_t ZERO = 0;
    static constexpr uint64_t ALL_MASK = ~ZERO;
    static constexpr int BITS = sizeof(uint64_t) * CHAR_BIT;
    static constexpr int ALL = BITS - 1;
    static constexpr int CAPACITY = (N + BITS - 1) / BITS;
    static constexpr int REMAINING = N & ALL;
    static constexpr uint64_t CLEANUP_MASK = ~(ALL_MASK << REMAINING);
    uint64_t a[CAPACITY];
    /**
     * storage:
     * a[0] = bit_63,   ..., bit_0
     * a[1] = bit_127,  ..., bit_64
     * ...
     *
     * semantics of <<= and >>= : usual <<= and >>= semantics for uint64_t
     */

    // NOTE: range violations are undefined behaviour and are unchecked
    struct reference {
       private:
        int pos;
        uint64_t* a;

       public:
        reference(int p, uint64_t* aa) : pos(p), a(aa) {}
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

    constexpr Bitset(uint64_t x) noexcept : Bitset() { a[0] = x; };

    constexpr Bitset(const std::string& x) : Bitset() {
#ifdef DEBUG
        if (x.size() > N * BITS)
            throw std::out_of_range("String is too large to convert to Bitset");
#endif
        int cur_loc = 0;
        int bit = 0;
        for (const auto c : x) {
#ifdef DEBUG
            if (c != '0' && c != '1')
                throw std::invalid_argument(
                    "String should consist of only 0s and 1s");
#endif
            a[cur_loc] |= uint64_t(c - '0') << (bit++);
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

    constexpr bool operator[](int pos) const {
        return (a[pos / BITS] >> (pos & ALL)) & 1;
    }

    reference operator[](int pos) {
#ifdef DEBUG
        assert(("Bitset index out of range, UB", pos >= 0 && pos < N));
#endif
        return reference(pos & ALL, a + pos / BITS);
    }

    std::string to_string() const {
        std::string s(N, '0');
        auto it = s.data() + N - 1;
        for (int i = 0; i < N; ++i) *(it--) += (a[i / BITS] >> (i & ALL)) & 1;
        return s;
    }

    int count() const noexcept {
        int ans = 0;
        for (int i = 0; i < CAPACITY; ++i) ans += __builtin_popcountll(a[i]);
        return ans;
    }

    int size() const noexcept { return N; }

#define IMPLEMENT(op)                                      \
    Bitset& operator op(const Bitset& b) noexcept {        \
        for (int i = 0; i < CAPACITY; ++i) a[i] op b.a[i]; \
        return *this;                                      \
    }
    IMPLEMENT(&=)
    IMPLEMENT(|=)
    IMPLEMENT(^=)
#undef IMPLEMENT
    Bitset& flip() noexcept {
        for (int i = 0; i < CAPACITY; ++i) a[i] = ~a[i];
        cleanup();
        return *this;
    }
    Bitset operator~() const noexcept { return Bitset(*this).flip(); }

    Bitset& operator<<=(int n) noexcept {
        if (n > N) {
            reset();
            return *this;
        }
        if (__builtin_expect(n != 0, 1)) {
            const int loc = n / BITS;
            const int pos = n & ALL;
            if (pos == 0)
                for (int i = CAPACITY - 1; i >= loc; --i) a[i] = a[i - loc];
            else {
                const int complement = BITS - pos;
                for (int i = CAPACITY - 1; i > loc; --i)
                    a[i] = (a[i - loc] << pos) | (a[i - loc - 1] >> complement);
                a[loc] = a[0] << pos;
            }
            std::fill(std::begin(a), std::begin(a) + loc, 0);
            cleanup();
        }
        return *this;
    }

    // 01000 -> 00100
    Bitset& operator>>=(int n) noexcept {
        if (n > N) {
            reset();
            return *this;
        }
        if (__builtin_expect(n != 0, 1)) {
            const int loc = n / BITS;
            const int pos = n & ALL;
            const int l = CAPACITY - 1 - loc;
            if (pos == 0)
                for (int i = 0; i <= l; ++i) a[i] = a[i + loc];
            else {
                const int complement = BITS - pos;
                for (int i = 0; i < l; ++i)
                    a[i] = (a[i + loc] >> pos) | (a[i + loc + 1] << complement);
                a[l] = a[CAPACITY - 1] >> pos;
            }
            std::fill(std::begin(a) + l + 1, std::end(a), 0);
            cleanup();
        }
        return *this;
    }

    Bitset operator<<(int n) const noexcept { return Bitset(*this) <<= n; }
    Bitset operator>>(int n) const noexcept { return Bitset(*this) >>= n; }

    void reset() noexcept { std::fill(std::begin(a), std::end(a), 0); }

    void set() noexcept {
        std::fill(std::begin(a), std::end(a), ALL_MASK);
        cleanup();
    }

    int find_first_set() const noexcept {
        for (int i = 0; i < CAPACITY; ++i) {
            uint64_t w = a[i];
            if (w) return i * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    int find_first_unset() const noexcept {
        for (int i = 0; i < CAPACITY; ++i) {
            uint64_t w = ~a[i];
            if (w) return i * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    int find_next_set(int i) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        int loc = i / BITS;
        uint64_t w = a[loc] & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        for (++loc; loc < CAPACITY; ++loc) {
            w = a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    int find_next_unset(int i) const noexcept {
        ++i;
        if (i >= BITS * CAPACITY) return N;
        int loc = i / BITS;
        uint64_t w = (~a[loc]) & (ALL_MASK << (i & ALL));
        if (w) return loc * BITS + __builtin_ctzll(w);
        for (++loc; loc < CAPACITY; ++loc) {
            w = ~a[loc];
            if (w) return loc * BITS + __builtin_ctzll(w);
        }
        return N;
    }

    int find_last_set() const noexcept {
        for (int i = CAPACITY - 1; i >= 0; --i) {
            uint64_t w = a[i];
            if (w) return i * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    int find_last_unset() const noexcept {
        for (int i = CAPACITY - 1; i >= 0; --i) {
            uint64_t w = ~a[i];
            if (w) return i * BITS + (ALL ^ __builtin_clzll(w));
        }
        return -1;
    }

    // int find_kth_set(int k) const noexcept {}

    // int find_kth_unset(int k) const noexcept {}

    // int find_kth_next_set(int i, int k) const noexcept {}

    // int find_kth_next_unset(int i, int k) const noexcept {}

    // int find_kth_last_set(int k) const noexcept {}

    // int find_kth_last_unset(int k) const noexcept {}

    // return [l, r)
    // 0 <= l < r <= N
    uint64_t slice(int l, int r) {
#ifdef DEBUG
        assert(l >= 0);
        assert(r >= 0);
        assert(l <= N);
        assert(r <= N);
#endif
        uint64_t ans = 0;
        if (l >= r) return ans;
        int i1 = l / BITS;
        int pos1 = l & ALL;
        int i2 = (r - 1) / BITS;
        int pos2 = (r - 1) & ALL;
        if (i1 == i2) {
            return (a[i1] & (ALL_MASK >> (ALL - pos2))) >> pos1;
        } else {
#ifdef DEBUG
            assert(i2 == i1 + 1);
#endif
            return (a[i1] << pos1) |
                   ((a[i2] & (ALL_MASK >> (ALL - pos2))) << (BITS - pos1));
        }
        return ans;
    }

    std::string to_string_debug() const {
        std::string s(CAPACITY * BITS, '0');
        for (int i = 0; i < CAPACITY * BITS; ++i)
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
