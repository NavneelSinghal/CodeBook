/*

// for sample usage, see https://codeforces.com/contest/1514/problem/D - highest memory submission
// https://users.dcc.uchile.cl/~gnavarro/ps/is14.pdf -- wavelet matrix is much faster

template <typename T>
struct wavelet_tree {
    struct node {
        T lo, hi;
        node *left_child, *right_child;
        std::vector<int> b;
        using iter = typename std::vector<T>::iterator;

        // numbers are in range [x, y]
        // array indices are [from, to)
        node(iter from, iter to, T x, T y) {
            lo = x, hi = y;
            left_child = nullptr;
            right_child = nullptr;
            if (lo == hi || from >= to)
                return;
            T mid = lo + (hi - lo) / 2;
            auto f = [mid](T e) { return e <= mid; };
            b.reserve(to - from + 1);
            b.push_back(0);
            for (auto it = from; it != to; it++)
                b.push_back(b.back() + f(*it));
            auto pivot = std::stable_partition(from, to, f);
            left_child = new node(from, pivot, lo, mid);
            right_child = new node(pivot, to, mid + 1, hi);
        }

        // kth smallest element in [l, r]
        T kth_smallest(int l, int r, int k) {
            if (l > r)
                return 0;
            if (lo == hi)
                return lo;
            // how many in the first (l-1) numbers that go left
            int lb = b[l - 1];
            // how many in first (r) numbers that go left
            int rb = b[r];
            int inLeft = rb - lb;
            if (k <= inLeft)
                return this->left_child->kth_smallest(lb + 1, rb, k);
            return this->right_child->kth_smallest(l - lb, r - rb, k - inLeft);
        }

        // count of numbers in [l, r] less than or equal to k
        // TODO: implement with range [k1, k2] instead of k
        int count_less_equal(int l, int r, T k) {
            if (l > r or k < lo)
                return 0;
            if (hi <= k)
                return r - l + 1;
            int lb = b[l - 1], rb = b[r];
            return this->left_child->count_less_equal(lb + 1, rb, k) +
                   this->right_child->count_less_equal(l - lb, r - rb, k);
        }

        // count of numbers in [l, r] equal to k
        int count_equal(int l, int r, T k) {
            if (l > r || k < lo || k > hi)
                return 0;
            if (lo == hi)
                return r - l + 1;
            int lb = b[l - 1], rb = b[r], mid = lo + (hi - lo) / 2;
            if (k <= mid)
                return this->left_child->count_equal(lb + 1, rb, k);
            return this->right_child->count_equal(l - lb, r - rb, k);
        }
        ~node() {
            delete left_child;
            delete right_child;
        }
    };
    vector<T> a;
    node *nd;
    wavelet_tree(vector<T> &A) {
        this->a = A;
        auto minmax_iterators =
            minmax_element(std::begin(this->a), std::end(this->a));
        this->nd =
            new node(std::begin(this->a), std::end(this->a),
                     *(minmax_iterators.first), *(minmax_iterators.second));
    }
    T kth_smallest(int l, int r, int k) {
        ++l, ++r;
        return nd->kth_smallest(l, r, k);
    }
    int count_less_equal(int l, int r, T k) {
        ++l, ++r;
        return nd->count_less_equal(l, r, k);
    }
    int count_equal(int l, int r, T k) {
        ++l, ++r;
        return nd->count_equal(l, r, k);
    }
    ~wavelet_tree() { delete this->nd; }
};
*/
