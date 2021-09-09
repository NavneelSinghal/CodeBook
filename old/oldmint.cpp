template <uint32_t Modulus>
class Modular {
    using M = Modular;

   public:
    static_assert(int(Modulus) >= 1, "Modulus must be in the range [1, 2^31)");
    static constexpr int modulus() { return Modulus; }
    static M raw(uint32_t v) { return *reinterpret_cast<M*>(&v); }
    Modular() : v_(0) {}
    Modular(int64_t v) : v_((v %= Modulus) < 0 ? v + Modulus : v) {}
    template <class T>
    explicit operator T() const {
        return v_;
    }
    M& operator++() { return v_ = ++v_ == Modulus ? 0 : v_, *this; }
    M& operator--() { return --(v_ ? v_ : v_ = Modulus), *this; }
    M operator+() const { return *this; }
    M operator-() const { return raw(v_ ? Modulus - v_ : 0); }
    M& operator*=(M o) { return v_ = uint64_t(v_) * o.v_ % Modulus, *this; }
    M& operator/=(M o) {
        auto [inv, gcd] = extgcd(o.v_, Modulus);
        assert(gcd == 1);
        return *this *= inv;
    }
    M& operator+=(M o) {
        return v_ = int(v_ += o.v_ - Modulus) < 0 ? v_ + Modulus : v_, *this;
    }
    M& operator-=(M o) {
        return v_ = int(v_ -= o.v_) < 0 ? v_ + Modulus : v_, *this;
    }
    friend M operator++(M& a, int) { return std::exchange(a, ++M(a)); }
    friend M operator--(M& a, int) { return std::exchange(a, --M(a)); }
    friend M operator*(M a, M b) { return a *= b; }
    friend M operator/(M a, M b) { return a /= b; }
    friend M operator+(M a, M b) { return a += b; }
    friend M operator-(M a, M b) { return a -= b; }
    friend std::istream& operator>>(std::istream& is, M& x) {
        int64_t v;
        return is >> v, x = v, is;
    }
    friend std::ostream& operator<<(std::ostream& os, M x) {
        return os << x.v_;
    }
    friend bool operator==(M a, M b) { return a.v_ == b.v_; }
    friend bool operator!=(M a, M b) { return a.v_ != b.v_; }

   private:
    static std::array<int, 2> extgcd(int a, int b) {
        std::array x{1, 0};
        while (b) std::swap(x[0] -= a / b * x[1], x[1]), std::swap(a %= b, b);
        return {x[0], a};
    }
    uint32_t v_;
};



// modular int library

#include <bits/stdc++.h>
using namespace std;

template <int MOD = 998'244'353>
struct Modular {
    using ll = long long;
    using M = Modular<MOD>;
    int value;
    static constexpr int MOD_value = MOD;
    M(ll v = 0) {
        value = v % MOD;
        if (value < 0) value += MOD;
    }
    M(ll a, ll b) : value(0) {
        *this += a;
        *this /= b;
    }
    M& operator+=(M const& b) {
        value += b.value;
        if (value >= MOD) value -= MOD;
        return *this;
    }
    M& operator-=(M const& b) {
        value -= b.value;
        if (value < 0) value += MOD;
        return *this;
    }
    M& operator*=(M const& b) {
        value = (ll)value * b.value % MOD;
        return *this;
    }
    friend M mexp(M a, ll e) {
        M res = 1;
        while (e) {
            if (e & 1) res *= a;
            a *= a;
            e >>= 1;
        }
        return res;
    }
    friend M inverse(M a) { return mexp(a, MOD - 2); }
    M& operator/=(M const& b) { return *this *= inverse(b); }
    friend M operator+(M a, M const b) { return a += b; }
    friend M operator-(M a, M const b) { return a -= b; }
    friend M operator-(M const a) { return 0 - a; }
    friend M operator*(M a, M const b) { return a *= b; }
    friend M operator/(M a, M const b) { return a /= b; }
    friend ostream& operator<<(ostream& os, M const& a) {
        return os << a.value;
    }
    friend bool operator==(M const& a, M const& b) {
        return a.value == b.value;
    }
    friend bool operator!=(M const& a, M const& b) {
        return a.value != b.value;
    }
};
using mint = Modular<mod>;
