template <class T, class D = void>
struct bigger_type {};
template <class T>
struct bigger_type<T, std::enable_if_t<sizeof(T) <= 4, void>> {
    using type = std::uint64_t;
};
template <class T>
struct bigger_type<T, std::enable_if_t<4 < sizeof(T) && sizeof(T) <= 8, void>> {
    using type = __uint128_t;
};

template <class T>
struct lehmer {
    using U = typename bigger_type<T>::type;
    using result_type = T;
    using state_type = U;
    state_type state;
    static constexpr T MAGIC =
        (sizeof(T) >= 8 ? 0xda942042e4dd58b5 : 0xe4dd58b5);
    static constexpr std::size_t shift_size = (sizeof(T) >= 8 ? 64 : 32);
    constexpr lehmer(state_type state_ = default_seed) : state{state_} {}
    constexpr result_type operator()() {
        state *= MAGIC;
        return state >> shift_size;
    }
    constexpr void seed(state_type s) { state = s; }
    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }
    constexpr friend bool operator==(const lehmer& a, const lehmer& b) {
        return a.state == b.state;
    }
    constexpr friend bool operator!=(const lehmer& a, const lehmer& b) {
        return a.state != b.state;
    }
    static constexpr state_type default_seed = 0;
};

template <class T>
struct wyhash {
    using U = typename bigger_type<T>::type;
    using result_type = T;
    using state_type = U;
    state_type state;
    static constexpr T MAGIC_INCREMENT =
        (sizeof(T) >= 8 ? 0x60bee2bee120fc15 : 0xe120fc15);
    static constexpr T MAGIC_MULTIPLY1 =
        (sizeof(T) >= 8 ? 0xa3b195354a39b70d : 0x4a39b70d);
    static constexpr T MAGIC_MULTIPLY2 =
        (sizeof(T) >= 8 ? 0x1b03738712fad5c9 : 0x12fad5c9);
    static constexpr std::size_t shift_size = (sizeof(T) >= 8 ? 64 : 32);

    constexpr wyhash(state_type state_ = default_seed) : state{state_} {}
    constexpr result_type operator()() {
        state += MAGIC_INCREMENT;
        U tmp = static_cast<U>(state) * MAGIC_MULTIPLY1;
        result_type m1 = static_cast<result_type>((tmp >> shift_size) ^ tmp);
        tmp = static_cast<U>(m1) * MAGIC_MULTIPLY2;
        result_type m2 = static_cast<result_type>((tmp >> shift_size) ^ tmp);
        return m2;
    }
    constexpr void discard(std::uint64_t steps) {
        state +=
            static_cast<state_type>(static_cast<U>(MAGIC_INCREMENT) * steps);
    }
    constexpr void seed(state_type s) { state = s; }
    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }
    constexpr friend bool operator==(const wyhash& a, const wyhash& b) {
        return a.state == b.state;
    }
    constexpr friend bool operator!=(const wyhash& a, const wyhash& b) {
        return a.state != b.state;
    }
    static constexpr state_type default_seed = 0;
};

template <class RNG>
struct Random : RNG {
    using RNG::operator();
    Random() : RNG(gen_seed()) {}

    static std::uint64_t gen_seed() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }

    using T = typename RNG::result_type;
    using U = typename bigger_type<T>::type;
    static constexpr std::size_t shift_size = CHAR_BIT * sizeof(T);

    // generate random integer in [0, N]
    auto generate_random_n(const T N) {
        U product = static_cast<U>(operator()() - RNG::min()) * N;
        T lsb = static_cast<T>(product);
        if (lsb < N) {
            T threshold = -static_cast<T>(N) % static_cast<T>(N);
            while (lsb < threshold) {
                product = static_cast<U>(operator()() - RNG::min()) * N;
                lsb = static_cast<T>(product);
            }
        }
        return static_cast<T>(product >> shift_size);
    }
    template <class Int>
    auto operator()(Int a, Int b)
        -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return a + static_cast<Int>(generate_random_n(b - a));
    }
    template <class Int>
    auto operator()(Int a) -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        return static_cast<Int>(generate_random_n(a - 1));
    }
    template <class Real>
    auto operator()(Real a, Real b)
        -> std::enable_if_t<std::is_floating_point_v<Real>, Real> {
        return std::uniform_real_distribution<Real>(a, b)(*this);
    }
};

using RandomMersenne32 = Random<std::mt19937>;
using RandomLehmer32 = Random<lehmer<std::uint32_t>>;
using RandomWyhash32 = Random<wyhash<std::uint32_t>>;
using RandomMersenne64 = Random<std::mt19937_64>;
using RandomLehmer64 = Random<lehmer<std::uint64_t>>;
using RandomWyhash64 = Random<wyhash<std::uint64_t>>;

