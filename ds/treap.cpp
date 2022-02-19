constexpr int mod = int(1e9) + 7;
constexpr int nttmod = 998'244'353;

template <std::uint32_t P>
struct ModInt32 {
   public:
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
    using m32 = ModInt32;
    using internal_value_type = u32;

   private:
    u32 v;
    static constexpr u32 get_r() {
        u32 iv = P;
        for (u32 i = 0; i != 4; ++i) iv *= 2U - P * iv;
        return -iv;
    }
    static constexpr u32 r = get_r(), r2 = -u64(P) % P;
    static_assert((P & 1) == 1);
    static_assert(-r * P == 1);
    static_assert(P < (1 << 30));
    static constexpr u32 pow_mod(u32 x, u64 y) {
        u32 res = 1;
        for (; y != 0; y >>= 1, x = u64(x) * x % P)
            if (y & 1) res = u64(res) * x % P;
        return res;
    }
    static constexpr u32 reduce(u64 x) {
        return (x + u64(u32(x) * r) * P) >> 32;
    }
    static constexpr u32 norm(u32 x) { return x - (P & -(x >= P)); }

   public:
    static constexpr u32 get_pr() {
        u32 tmp[32] = {}, cnt = 0;
        const u64 phi = P - 1;
        u64 m = phi;
        for (u64 i = 2; i * i <= m; ++i)
            if (m % i == 0) {
                tmp[cnt++] = i;
                while (m % i == 0) m /= i;
            }
        if (m != 1) tmp[cnt++] = m;
        for (u64 res = 2; res != P; ++res) {
            bool flag = true;
            for (u32 i = 0; i != cnt && flag; ++i)
                flag &= pow_mod(res, phi / tmp[i]) != 1;
            if (flag) return res;
        }
        return 0;
    }
    constexpr ModInt32() : v(0){};
    ~ModInt32() = default;
    constexpr ModInt32(u32 _v) : v(reduce(u64(_v) * r2)) {}
    constexpr ModInt32(i32 _v) : v(reduce(u64(_v % i64(P) + i64(P)) * r2)) {}
    constexpr ModInt32(u64 _v) : v(reduce((_v % P) * r2)) {}
    constexpr ModInt32(i64 _v) : v(reduce(u64(_v % i64(P) + i64(P)) * r2)) {}
    constexpr ModInt32(const m32& rhs) : v(rhs.v) {}
    constexpr u32 get() const { return norm(reduce(v)); }
    explicit constexpr operator u32() const { return get(); }
    explicit constexpr operator i32() const { return i32(get()); }
    constexpr m32& operator=(const m32& rhs) { return v = rhs.v, *this; }
    constexpr m32 operator-() const {
        m32 res;
        return res.v = (P << 1 & -(v != 0)) - v, res;
    }
    constexpr m32 inv() const { return pow(P - 2); }
    constexpr m32& operator+=(const m32& rhs) {
        return v += rhs.v - (P << 1), v += P << 1 & -(v >> 31), *this;
    }
    constexpr m32& operator-=(const m32& rhs) {
        return v -= rhs.v, v += P << 1 & -(v >> 31), *this;
    }
    constexpr m32& operator*=(const m32& rhs) {
        return v = reduce(u64(v) * rhs.v), *this;
    }
    constexpr m32& operator/=(const m32& rhs) {
        return this->operator*=(rhs.inv());
    }
    constexpr friend m32 operator+(const m32& lhs, const m32& rhs) {
        return m32(lhs) += rhs;
    }
    constexpr friend m32 operator-(const m32& lhs, const m32& rhs) {
        return m32(lhs) -= rhs;
    }
    constexpr friend m32 operator*(const m32& lhs, const m32& rhs) {
        return m32(lhs) *= rhs;
    }
    constexpr friend m32 operator/(const m32& lhs, const m32& rhs) {
        return m32(lhs) /= rhs;
    }
    constexpr friend bool operator==(const m32& lhs, const m32& rhs) {
        return norm(lhs.v) == norm(rhs.v);
    }
    constexpr friend bool operator!=(const m32& lhs, const m32& rhs) {
        return norm(lhs.v) != norm(rhs.v);
    }
    friend std::istream& operator>>(std::istream& is, m32& rhs) {
        return is >> rhs.v, rhs.v = reduce(u64(rhs.v) * r2), is;
    }
    friend std::ostream& operator<<(std::ostream& os, const m32& rhs) {
        return os << rhs.get();
    }
    constexpr m32 pow(i64 y) const {
        // assumes P is a prime
        i64 rem = y % (P - 1);
        if (y > 0 && rem == 0)
            y = P - 1;
        else
            y = rem;
        m32 res(1), x(*this);
        for (; y != 0; y >>= 1, x *= x)
            if (y & 1) res *= x;
        return res;
    }
};

using mint = ModInt32<nttmod>;

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
        // return a + static_cast<Int>(generate_random_n(b - a));
        return std::uniform_int_distribution<Int>(a, b)(*this);
    }
    template <class Int>
    auto operator()(Int a) -> std::enable_if_t<std::is_integral_v<Int>, Int> {
        // return static_cast<Int>(generate_random_n(a - 1));
        return std::uniform_int_distribution<Int>(0, a - 1)(*this);
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

namespace treap {

    // to work with parents, uncomment the ///

    RandomWyhash32 rng;

    using T = mint;

    struct U {
        mint b, c;
        constexpr U() : b{1}, c{0} {}
        constexpr U(const mint& b_, const mint& c_) : b{b_}, c{c_} {}
        constexpr bool operator==(const U& other) const {
            return b == other.b && c == other.c;
        }
        constexpr bool operator!=(const U& other) const {
            return b != other.b || c != other.c;
        }
    };

    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        /// Node* parent = nullptr;

        int size = 0;
        T value = 0;
        T sum = 0;
        U update{};
        bool to_reverse = false;

        constexpr Node(T value_ = 0)
            : size{1},
              value{value_},
              sum{value_},
              update{},
              to_reverse{false} {}
    };

    int size(Node* a) { return a ? a->size : 0; }
    T value(Node* a) { return a ? a->value : 0; }
    T sum(Node* a) { return a ? a->sum : 0; }
    U update(Node* a) { return a ? a->update : U{}; }
    bool to_reverse(Node* a) { return a ? a->to_reverse : false; }

    /// Node* parent(Node* a) { return a ? a->parent : nullptr; }

    // applies the lazy stuff to current position (i.e., does apply_update) and
    // pushes it down to children (i.e., does compose_update)
    // assumes a is not nullptr
    void push(Node* a) {
        if (a->update != U{}) {
            // apply update here

            // size, value, sum, update, to_reverse
            a->value = a->update.b * a->value + a->update.c;
            a->sum = a->update.b * a->sum + a->update.c * a->size;

            constexpr auto compose_update = [](const U& first,
                                               const U& second) -> U {
                // b2(b1 x + c1) + c2
                // b1 b2 x + (b2 c1 + c2)
                return U{first.b * second.b, second.b * first.c + second.c};
            };

            if (a->left)
                a->left->update = compose_update(a->left->update, a->update);
            if (a->right)
                a->right->update = compose_update(a->right->update, a->update);
            a->update = U{};
        }
        if (a->to_reverse) {
            swap(a->left, a->right);
            if (a->left) a->left->to_reverse ^= 1;
            if (a->right) a->right->to_reverse ^= 1;
            a->to_reverse = false;
        }
    }

    // recomputes values from children
    // there might be lazy values that need to be propagated down in node a, but
    // we don't care about them as they need to be applied on the whole subarray
    // corresponding to the subtree of a, so whenever they are applied, they
    // will lead to the correct answer (as apply_update distributes over
    // combine_nodes)
    // assumes a is not nullptr
    void recalc(Node* a) {
        // size, value, sum, update, to_reverse
        a->size = 1;
        a->sum = a->value;

        constexpr auto combine_into = [](const Node* a, const Node* b,
                                         Node* res) {
            int size = a->size + b->size;
            mint sum = a->sum + b->sum;
            res->size = size;
            res->sum = sum;
        };

        if (a->left) push(a->left), combine_into(a->left, a, a);
        if (a->right) push(a->right), combine_into(a, a->right, a);
    }

    /// Node* root(Node* a) {
    ///     while (parent(a)) a = parent(a);
    ///     return a;
    /// }

    // takes a subtree and splits it into two subtrees
    // not necessary for it to have update = id_update
    [[nodiscard]] std::pair<Node*, Node*> split(Node* a, int in_left) {
        if (size(a) <= in_left) return {a, nullptr};
        push(a);
        /// assert(!a->parent);
        int size_left = size(a->left);
        if (size_left >= in_left) {
            /// if (a->left) a->left->parent = nullptr;
            auto [l, r] = split(a->left, in_left);
            a->left = r;
            /// if (r) r->parent = a;
            recalc(a);
            return {l, a};
        } else {
            /// if (a->right) a->right->parent = nullptr;
            auto [l, r] = split(a->right, in_left - size_left - 1);
            a->right = l;
            /// if (l) l->parent = a;
            recalc(a);
            return {a, r};
        }
    }

    // takes two nodes and merges them together
    // not necessary for them to have update = id_update
    [[nodiscard]] Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        /// assert(!(a->parent));
        /// assert(!(b->parent));
        if (rng(a->size + b->size) < b->size) {
            push(b);
            /// if (b->left) b->left->parent = nullptr;
            b->left = merge(a, b->left);
            /// if (b->left) b->left->parent = b;
            recalc(b);
            return b;
        } else {
            push(a);
            /// if (a->right) a->right->parent = nullptr;
            a->right = merge(a->right, b);
            /// if (a->right) a->right->parent = a;
            recalc(a);
            return a;
        }
    }

    // applies update to a node
    [[nodiscard]] Node* update(Node* a, U u) {
        if (!a) return a;
        constexpr auto compose_update = [](const U& first,
                                           const U& second) -> U {
            // b2(b1 x + c1) + c2
            // b1 b2 x + (b2 c1 + c2)
            return U{first.b * second.b, second.b * first.c + second.c};
        };
        a->update = compose_update(a->update, u);
        return a;
    }

    // applies reverse to a node
    [[nodiscard]] Node* reverse(Node* a) {
        if (!a) return a;
        a->to_reverse ^= 1;
        return a;
    }

    // queries a node
    [[nodiscard]] T query(Node* a) {
        if (!a) return T{};
        push(a);
        return a->sum;
    }

    void traverse(Node* a, vector<T>& v) {
        if (!a) return;
        push(a);
        traverse(a->left, v);
        // push stuff
        v.push_back(a->value);
        traverse(a->right, v);
    }

    template <class ostream>
    void dbg(Node* a, ostream& os) {
        if (!a) return;
        push(a);
        dbg(a->left, os);
        // print stuff
        os << a->value << ' ';
        dbg(a->right, os);
    }

    template <class ostream>
    ostream& operator<<(ostream& os, Node* a) {
        dbg(a, os);
        return os;
    }

    void delete_treap(Node* a) {
        if (!a) return;
        delete_treap(a->left);
        delete_treap(a->right);
        delete a;
    }

}  // namespace treap


/*

   another example: (don't copy paste directly though)

struct Random : std::mt19937 {
    using std::mt19937::mt19937;
    using std::mt19937::operator();
    static int64_t gen_seed() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
    Random() : std::mt19937(gen_seed()) {}
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

namespace treap {

    // to work with parents, uncomment the ///

    Random rng;

    using T = int;

    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        /// Node* parent = nullptr;

        int size = 0;

        T value = 0;
        T sum = 0;
        T leftmost_value = -1;
        T rightmost_value = -1;
        T longest_prefix = 0;
        T longest_suffix = 0;

        T to_assign = -1;
        bool xor_with = false;
        bool to_reverse = false;

        T longest_contiguous = 0;

        constexpr Node(T value_ = 0)
            : size{1},
              value{value_},
              sum{value_},
              leftmost_value{value_},
              rightmost_value{value_},
              longest_prefix{1},
              longest_suffix{1},
              to_assign{-1},
              xor_with{false},
              to_reverse{false},
              longest_contiguous{1} {}
    };

    int size(Node* a) { return a ? a->size : 0; }
    T value(Node* a) { return a ? a->value : 0; }
    T sum(Node* a) { return a ? a->sum : 0; }
    T leftmost_value(Node* a) { return a ? a->leftmost_value : -1; }
    T rightmost_value(Node* a) { return a ? a->rightmost_value : -1; }
    T longest_prefix(Node* a) { return a ? a->longest_prefix : 0; }
    T longest_suffix(Node* a) { return a ? a->longest_suffix : 0; }
    T to_assign(Node* a) { return a ? a->to_assign : -1; }
    bool xor_with(Node* a) { return a ? a->xor_with : false; }
    bool to_reverse(Node* a) { return a ? a->to_reverse : false; }
    T longest_contiguous(Node* a) { return a ? a->longest_contiguous : 0; }

    /// Node* parent(Node* a) { return a ? a->parent : nullptr; }

    // pushes the lazy stuff to children and updates values
    void push(Node* a) {
        if (!a) return;
        if (a->to_assign != -1) {
            a->value = a->to_assign;
            a->sum = a->to_assign * a->size;
            a->leftmost_value = a->to_assign;
            a->rightmost_value = a->to_assign;
            a->longest_prefix = a->size;
            a->longest_suffix = a->size;
            a->longest_contiguous = a->size;
            if (a->left) {
                a->left->to_reverse = false;
                a->left->xor_with = false;
                a->left->to_assign = a->to_assign;
            }
            if (a->right) {
                a->right->to_reverse = false;
                a->right->xor_with = false;
                a->right->to_assign = a->to_assign;
            }
            a->to_assign = -1;
        }
        if (a->xor_with) {
            a->value ^= 1;
            a->sum = a->size - a->sum;
            a->leftmost_value ^= 1;
            a->rightmost_value ^= 1;
            if (a->left) a->left->xor_with ^= 1;
            if (a->right) a->right->xor_with ^= 1;
            a->xor_with = false;
        }
        if (a->to_reverse) {
            swap(a->left, a->right);
            swap(a->leftmost_value, a->rightmost_value);
            swap(a->longest_prefix, a->longest_suffix);
            if (a->left) a->left->to_reverse ^= 1;
            if (a->right) a->right->to_reverse ^= 1;
            a->to_reverse = false;
        }
    }

    // recomputes values from children
    // assumes there is no lazy in a that needs to be pushed down to children
    void recalc(Node* a) {
        if (!a) return;

        a->size = 1;
        a->sum = a->value;
        a->leftmost_value = a->value;
        a->rightmost_value = a->value;
        a->longest_prefix = 1;
        a->longest_suffix = 1;
        a->longest_contiguous = 1;

        // assert(a->to_assign == -1);
        // assert(!a->xor_with);
        // assert(!a->to_reverse);

        auto combine_into = [](const Node* a, const Node* b, Node* res) {
            int longest_prefix = a->longest_prefix;
            int longest_suffix = b->longest_suffix;
            int longest_contiguous =
                std::max(a->longest_contiguous, b->longest_contiguous);
            if (a->rightmost_value == b->leftmost_value) {
                if (longest_prefix == a->size)
                    longest_prefix += b->longest_prefix;
                if (longest_suffix == b->size)
                    longest_suffix += a->longest_suffix;
                longest_contiguous = std::max(
                    longest_contiguous, a->longest_suffix + b->longest_prefix);
            }
            res->size = a->size + b->size;
            res->sum = a->sum + b->sum;
            res->leftmost_value = a->leftmost_value;
            res->rightmost_value = b->rightmost_value;
            res->longest_prefix = longest_prefix;
            res->longest_suffix = longest_suffix;
            res->longest_contiguous = longest_contiguous;
        };

        if (a->left) {
            push(a->left);
            combine_into(a->left, a, a);
        }
        if (a->right) {
            push(a->right);
            combine_into(a, a->right, a);
        }
    }

    /// Node* root(Node* a) {
    ///     while (parent(a)) a = parent(a);
    ///     return a;
    /// }

    [[nodiscard]] std::pair<Node*, Node*> split(Node* a, int in_left) {
        if (size(a) <= in_left) return {a, nullptr};
        assert(a);
        push(a);
        /// assert(!a->parent);
        int size_left = size(a->left);
        if (size_left >= in_left) {
            auto a_left = a->left;
            /// if (a_left) a_left->parent = nullptr;
            auto [l, r] = split(a_left, in_left);
            a->left = r;
            /// if (r) r->parent = a;
            recalc(a);
            return {l, a};
        } else {
            auto a_right = a->right;
            /// if (a_right) a_right->parent = nullptr;
            auto [l, r] = split(a_right, in_left - size_left - 1);
            a->right = l;
            /// if (l) l->parent = a;
            recalc(a);
            return {a, r};
        }
    }

    [[nodiscard]] Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        /// assert(!(a->parent));
        /// assert(!(b->parent));
        push(a);
        push(b);
        if (rng(a->size + b->size) < b->size) {
            auto b_left = b->left;
            /// if (b_left) b_left->parent = nullptr;
            b->left = merge(a, b_left);
            /// if (b->left) b->left->parent = b;
            recalc(b);
            return b;
        } else {
            auto a_right = a->right;
            /// if (a_right) a_right->parent = nullptr;
            a->right = merge(a_right, b);
            /// if (a->right) a->right->parent = a;
            recalc(a);
            return a;
        }
    }

    [[nodiscard]] Node* xor_with_one(Node* a) {
        if (!a) return a;
        a->xor_with ^= 1;
        push(a);
        return a;
    }

    [[nodiscard]] Node* reverse(Node* a) {
        if (!a) return a;
        a->to_reverse ^= 1;
        push(a);
        return a;
    }

    [[nodiscard]] Node* sort(Node* a) {
        if (!a) return a;
        T count_zeros = a->size - a->sum;
        auto [a1, a2] = split(a, count_zeros);
        if (a1) a1->to_assign = 0;
        if (a2) a2->to_assign = 1;
        return merge(a1, a2);
    }

    void traverse(Node* a, vector<T>& v) {
        if (!a) return;
        traverse(a->left, v);
        v.push_back(a->value);
        traverse(a->right, v);
    }

    void dbg(Node* a, ostream& os) {
        if (!a) return;
        dbg(a->left, os);
        os << a->value << ' ';
        dbg(a->right, os);
    }
    ostream& operator<<(ostream& os, Node* a) {
        dbg(a, os);
        return os;
    }

    void delete_treap(Node* a) {
        if (!a) return;
        delete_treap(a->left);
        delete_treap(a->right);
        delete a;
    }

}  // namespace treap
*/
