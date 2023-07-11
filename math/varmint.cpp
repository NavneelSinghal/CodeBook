template <class T, class D = void>
struct bigger_type {};
template <class T>
struct bigger_type<T, std::enable_if_t<sizeof(T) <= 4, void>> {
    using type = std::int64_t;
};
template <class T>
struct bigger_type<T, std::enable_if_t<4 < sizeof(T) && sizeof(T) <= 8, void>> {
    using type = __int128_t;
};

template <class T>
class Mint {
    using M = Mint;
    using Base = typename decay<decltype(T::value)>::type;
    using Bigger = typename bigger_type<Base>::type;
    static constexpr Base Zero = Base{0};
    static constexpr Base One = Base{1};

   public:
    static constexpr Base mod() { return T::value; }
    constexpr Mint() : v_(Zero) {}
    static constexpr M raw(Base v) { return *reinterpret_cast<M*>(&v); }
    template <class V>
    constexpr Mint(V v) : v_{Base((v %= mod()) < Zero ? v + mod() : v)} {}
    constexpr M& operator++() { return v_ = ++v_ == mod() ? Zero : v_, *this; }
    constexpr M& operator--() { return --(v_ ? v_ : v_ = mod()), *this; }
    constexpr M operator+() const { return *this; }
    constexpr M operator-() const { return raw(v_ ? mod() - v_ : Zero); }
    constexpr M& operator*=(M o) {
        return v_ = Bigger(v_) * o.v_ % mod(), *this;
    }
    constexpr M& operator/=(M o) {
        auto [inv, gcd] = extgcd(o.v_, mod());
        assert(gcd == One);
        return *this *= inv;
    }
    constexpr M& operator+=(M o) {
        return v_ = Base(v_ += o.v_ - mod()) < Zero ? v_ + mod() : v_, *this;
    }
    constexpr M& operator-=(M o) {
        return v_ = Base(v_ -= o.v_) < Zero ? v_ + mod() : v_, *this;
    }
    constexpr friend M operator++(M& a, int) {
        return std::exchange(a, ++M(a));
    }
    constexpr friend M operator--(M& a, int) {
        return std::exchange(a, --M(a));
    }
    constexpr friend M operator*(M a, M b) { return a *= b; }
    constexpr friend M operator/(M a, M b) { return a /= b; }
    constexpr friend M operator+(M a, M b) { return a += b; }
    constexpr friend M operator-(M a, M b) { return a -= b; }
    constexpr friend std::istream& operator>>(std::istream& is, M& x) {
        int64_t v;
        return is >> v, x = v, is;
    }
    constexpr friend std::ostream& operator<<(std::ostream& os, M x) {
        return os << x.v_;
    }
    constexpr friend bool operator==(M a, M b) { return a.v_ == b.v_; }
    constexpr friend bool operator!=(M a, M b) { return a.v_ != b.v_; }

   private:
    static constexpr std::array<Base, 2> extgcd(Base a, Base b) {
        std::array x{One, Zero};
        while (b) std::swap(x[0] -= a / b * x[1], x[1]), std::swap(a %= b, b);
        return {x[0], a};
    }
    Base v_;
};

constexpr int md = static_cast<int>(1e9 + 7);
using mint_constant =
    Mint<std::integral_constant<decay<decltype(md)>::type, md>>;

constexpr int64_t md64 = static_cast<int64_t>(1e9 + 7);
using mint_constant_64 =
    Mint<std::integral_constant<decay<decltype(md64)>::type, md64>>;

using ModType = int;
struct VarMod {
    static ModType value;
    using type = ModType;
};
ModType VarMod::value;
ModType& mod_var = VarMod::value;
using mint_variable = Mint<VarMod>;

using ModType64 = int64_t;
struct VarMod64 {
    static ModType64 value;
    using type = ModType64;
};
ModType64 VarMod64::value;
ModType64& mod_var_64 = VarMod64::value;
using mint_variable_64 = Mint<VarMod64>;

