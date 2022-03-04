template <class T>
struct deque_allocator {
    std::deque<T> d;
    template <typename... Args>
    T* allocate(Args&&... args) {
        d.emplace_back(args...);
        return &d.back();
    }
};

template <class Info>
struct LiChaoTree {
    using T = typename Info::T;
    struct Line {
        T a, b;
        Line(T a, T b) : a(a), b(b) {}
        Line() {
        }  // only for exploiting uninitialized optimizations for the buffer
        inline T operator()(T x) const { return a * x + b; }
    };

    struct Node {
        Line x;
        Node *l, *r;
        Node(const Line& x) : x{x}, l{nullptr}, r{nullptr} {}
        Node() {
        }  // only for exploiting uninitialized optimizations for the buffer
    };

    template <class U>
    Node* newNode(U&& x) {
        static deque_allocator<Node> allocator;
        return allocator.allocate(std::forward<Node>(x));
    }

    Node* root;

    LiChaoTree() : root{nullptr} {}

    Node* update(Node* t, Line& x, const T& l, const T& r, const T& x_l,
                 const T& x_r) {
        if (!t) return newNode(x);
        auto t_l = t->x(l), t_r = t->x(r);
        if (!Info::cmp(x_l, t_l) && !Info::cmp(x_r, t_r)) {
            return t;
        } else if (!Info::cmp(t_l, x_l) && !Info::cmp(t_r, x_r)) {
            t->x = x;
            return t;
        } else {
            T m = (l + r) / 2;
            auto t_m = t->x(m), x_m = x(m);
            if (Info::cmp(x_m, t_m)) {
                std::swap(t->x, x);
                if (!Info::cmp(x_l, t_l))
                    t->l = update(t->l, x, l, m, t_l, t_m);
                else
                    t->r = update(t->r, x, m + 1, r, t_m + x.a, t_r);
            } else {
                if (!Info::cmp(t_l, x_l))
                    t->l = update(t->l, x, l, m, x_l, x_m);
                else
                    t->r = update(t->r, x, m + 1, r, x_m + x.a, x_r);
            }
            return t;
        }
    }

    void update(const T& a, const T& b) {
        Line x(a, b);
        root = update(root, x, Info::x_low, Info::x_high, x(Info::x_low),
                      x(Info::x_high));
    }

    T query(const Node* t, const T& l, const T& r, const T& x) const {
        if (!t) return Info::id;
        if (l == r) return t->x(x);
        T m = (l + r) / 2;
        if (x <= m)
            return std::min(t->x(x), query(t->l, l, m, x), Info::cmp);
        else
            return std::min(t->x(x), query(t->r, m + 1, r, x), Info::cmp);
    }

    T query(const T& x) const {
        return query(root, Info::x_low, Info::x_high, x);
    }
};

using ll = std::int64_t;

struct LiChaoInfo {
    using T = ll;
    static constexpr T x_low = -ll(1e9) - 1;
    static constexpr T x_high = ll(1e9) + 1;
    // min: 8e18, max: -8e18
    static constexpr T id = -ll(8e18);
    // min: x < y, max: x > y
    static constexpr bool cmp(T x, T y) { return x > y; }
};

using LiChao = LiChaoTree<LiChaoInfo>;

