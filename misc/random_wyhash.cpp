struct wyhash_64 {
    using result_type = std::uint64_t;
    using state_type = std::uint64_t;
    state_type state;
    constexpr wyhash_64(std::uint64_t state_ = default_seed) : state{state_} {}
    constexpr result_type operator()() {
        state += 0x60bee2bee120fc15;
        __uint128_t tmp = static_cast<__uint128_t>(state) * 0xa3b195354a39b70d;
        result_type m1 = static_cast<result_type>((tmp >> 64) ^ tmp);
        tmp = static_cast<__uint128_t>(m1) * 0x1b03738712fad5c9;
        result_type m2 = static_cast<result_type>((tmp >> 64) ^ tmp);
        return m2;
    }
    constexpr void discard(std::uint64_t steps) {
        state += static_cast<state_type>(
            static_cast<__uint128_t>(0x60bee2bee120fc15) * steps);
    }
    constexpr void seed(state_type s) { state = s; }
    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }
    constexpr friend bool operator==(const wyhash_64& a, const wyhash_64& b) {
        return a.state == b.state;
    }
    constexpr friend bool operator!=(const wyhash_64& a, const wyhash_64& b) {
        return a.state != b.state;
    }
    static constexpr state_type default_seed = 0;
};

struct Random : wyhash_64 {
    using wyhash_64::operator();
    static int64_t gen_seed() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
    Random() : wyhash_64(gen_seed()) {}
    template <class Int>
    auto operator()(Int a, Int b)
        -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return std::uniform_int_distribution<Int>(a, b)(*this);
    }
    template <class Int>
    auto operator()(Int a) -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return std::uniform_int_distribution<Int>(0, a - 1)(*this);
    }
    template <class Real>
    auto operator()(Real a, Real b)
        -> std::enable_if_t<std::is_floating_point_v<Real>, Real> {
        return std::uniform_real_distribution<Real>(a, b)(*this);
    }
};

