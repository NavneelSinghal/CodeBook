// BUF should be enough to store O((N + Q) log (range)) nodes
// Don't make more than one template specialization for the lichao tree since
// otherwise the buffer will be pretty huge
// uses two buffers - one for each template
template <class T, T x_low, T x_high, T id, int BUF>
struct LiChaoTree {
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
    Node* newNode(const U& x) {
        static int bufSize = BUF;
        static Node buf[BUF];

        assert(bufSize);
        buf[--bufSize] = Node(x);
        return &buf[bufSize];
    }

    Node* root;

    LiChaoTree() : root{nullptr} {}

    Node* update(Node* t, Line& x, const T& l, const T& r, const T& x_l,
                 const T& x_r) {
        if (!t) return newNode(x);
        auto t_l = t->x(l), t_r = t->x(r);
        if (t_l <= x_l && t_r <= x_r) {
            return t;
        } else if (t_l >= x_l && t_r >= x_r) {
            t->x = x;
            return t;
        } else {
            T m = (l + r) / 2;
            auto t_m = t->x(m), x_m = x(m);
            if (t_m > x_m) {
                std::swap(t->x, x);
                if (x_l >= t_l)
                    t->l = update(t->l, x, l, m, t_l, t_m);
                else
                    t->r = update(t->r, x, m + 1, r, t_m + x.a, t_r);
            } else {
                if (t_l >= x_l)
                    t->l = update(t->l, x, l, m, x_l, x_m);
                else
                    t->r = update(t->r, x, m + 1, r, x_m + x.a, x_r);
            }
            return t;
        }
    }

    void update(const T& a, const T& b) {
        Line x(a, b);
        root = update(root, x, x_low, x_high, x(x_low), x(x_high));
    }

    T query(const Node* t, const T& l, const T& r, const T& x) const {
        if (!t) return id;
        if (l == r) return t->x(x);
        T m = (l + r) / 2;
        if (x <= m)
            return std::min(t->x(x), query(t->l, l, m, x));
        else
            return std::min(t->x(x), query(t->r, m + 1, r, x));
    }

    T query(const T& x) const { return query(root, x_low, x_high, x); }
};

