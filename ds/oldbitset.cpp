/*
 * TODO:
 all the usual stuff +:
- first set bit
- last set bit
- first unset bit
- last unset bit
- splicing a range to use as a mask
*/

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>

template <int N>
struct bitset {
   private:
    static constexpr int BITS = sizeof(uint64_t) * CHAR_BIT;
    static constexpr int ALL = BITS - 1;
    static constexpr uint64_t one = 1;
    static constexpr int CAPACITY = (N + ALL) / BITS;
    static constexpr int REMAINING = ALL ^ (ALL & ((N - 1) % BITS));
    uint64_t a[CAPACITY];

    template <typename T, T m, int k>
    constexpr static inline T swapbits(T p) {
        T q = ((p >> k) ^ p) & m;
        return p ^ q ^ (q << k);
    }

    constexpr static uint64_t kbitreverse(uint64_t n) {
        constexpr uint64_t m0 = 0x5555555555555555LLU;
        constexpr uint64_t m1 = 0x0300c0303030c303LLU;
        constexpr uint64_t m2 = 0x00c0300c03f0003fLLU;
        constexpr uint64_t m3 = 0x00000ffc00003fffLLU;
        n = ((n >> 1) & m0) | (n & m0) << 1;
        n = swapbits<uint64_t, m1, 4>(n);
        n = swapbits<uint64_t, m2, 8>(n);
        n = swapbits<uint64_t, m3, 20>(n);
        n = (n >> 34) | (n << 30);
        return n;
    }

    struct reference {
       private:
        int loc, pos;
        uint64_t* a;

       public:
        reference(int l, int p, uint64_t* a) : loc(l), pos(p), a(a) {}
        reference& operator=(bool x) noexcept {
            if (x)
                a[loc] |= one << pos;
            else
                a[loc] &= ~(one << pos);
            return *this;
        }
        reference& operator=(const reference& x) noexcept {
            if (((x.a)[x.loc] >> x.pos) & 1)
                a[loc] |= one << pos;
            else
                a[loc] &= ~(one << pos);
            return *this;
        }
        operator bool() const noexcept { return (a[loc] >> pos) & 1; }
        bool operator~() const noexcept { return !((a[loc] >> pos) & 1); }
        reference& flip() noexcept { a[loc] ^= one << pos; }
    };

    __attribute__((target("popcnt"))) int popcnt_ll(uint64_t x) const {
        return __builtin_popcountll(x);
    }

   public:
    constexpr bitset() noexcept {
        static_assert(BITS == 64);
        static_assert(kbitreverse(1) == (one << 63));
        std::fill(std::begin(a), std::end(a), 0);
    };

    constexpr bitset(uint64_t x) noexcept : bitset() { a[0] = kbitreverse(x); };

    constexpr bitset(const std::string& x) : bitset() {
        if (x.size() > N * BITS)
            throw std::out_of_range("String is too large to convert to bitset");
        int cur_loc = 0;
        int bit = ALL;
        for (const auto&& c : x) {
#ifdef DEBUG
            if (c != '0' && c != '1')
                throw std::invalid_argument(
                    "String should consist of only 0s and 1s");
#endif
            a[cur_loc] |= uint64_t(c - '0') << (bit--);
            if (!(~bit)) bit = ALL, ++cur_loc;
        }
    }

    bool operator==(const bitset& b) const {
        if constexpr (CAPACITY * BITS == N)
            return std::equal(std::begin(a), std::end(a), std::begin(b.a));
        bool val = std::equal(std::begin(a), std::end(a) - 1, std::begin(b.a));
        return val && ((a[CAPACITY - 1] >> REMAINING) ==
                       (b.a[CAPACITY - 1] >> REMAINING));
    }

    bool operator!=(const bitset& b) const { return !(*this == b); }

    constexpr bool operator[](int pos) const {
        return (a[pos / BITS] >> (ALL ^ (pos & ALL))) & 1;
    }

    reference operator[](int pos) {
        return reference(pos / BITS, ALL ^ (pos & ALL), a);
    }

    std::string to_string() const {
        std::string s(N, '0');
        for (int i = 0; i < N; ++i)
            s[i] += (a[i / BITS] >> (ALL ^ (i & ALL))) & 1;
        return s;
    }

    std::size_t count() const noexcept {
        std::size_t ans = 0;
        for (int i = 0; i < CAPACITY - 1; ++i) ans += popcnt_ll(a[i]);
        ans += popcnt_ll(a[CAPACITY - 1] >> REMAINING);
        return ans;
    }

    std::size_t size() const noexcept { return N; }

#define IMPLEMENT(op)                                      \
    bitset& operator op(const bitset& b) noexcept {        \
        for (int i = 0; i < CAPACITY; ++i) a[i] op b.a[i]; \
        return *this;                                      \
    }
    IMPLEMENT(&=)
    IMPLEMENT(|=)
    IMPLEMENT(^=)
#undef IMPLEMENT
    bitset& flip() noexcept {
        for (int i = 0; i < CAPACITY; ++i) a[i] = ~a[i];
        return *this;
    }
    bitset& operator~() const noexcept { return bitset<N>(*this).flip(); }
    
    bitset& operator<<=(int n) noexcept {
        int loc = n / BITS;
        int pos = n % BITS;
        // do cancer stuff
        return *this;
    }

    // void print_raw() const {
    //     for (int i = 0; i < (N + BITS - 1) / BITS; ++i) std::cout << a[i] <<
    //     ' '; std::cout << '\n';
    // }
};

int main() {
    bitset<100> a(1);
    bitset<100> b;
    // assert(a == b);
    std::cout << a.to_string() << '\n';
    a[0] = 1;
    std::cout << a.to_string() << '\n';
    b[1] = 1;
    std::cout << b.to_string() << '\n';
    b[0] = a[0];
    std::cout << b.to_string() << '\n';
    a ^= b;
    std::cout << a.to_string() << '\n';
    bitset<32> aa, ab;
    aa[32] = 1;
    std::cout << aa.count() << '\n';
    assert(aa == ab);
    return 0;
}
