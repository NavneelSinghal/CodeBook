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

    [[no_discard]] std::pair<Node*, Node*> split(Node* a, int in_left) {
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

    [[no_discard]] Node* merge(Node* a, Node* b) {
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

    [[no_discard]] Node* xor_with_one(Node* a) {
        if (!a) return a;
        a->xor_with ^= 1;
        push(a);
        return a;
    }

    [[no_discard]] Node* reverse(Node* a) {
        if (!a) return a;
        a->to_reverse ^= 1;
        push(a);
        return a;
    }

    [[no_discard]] Node* sort(Node* a) {
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

