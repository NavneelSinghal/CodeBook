#include "bits/stdc++.h"

template <int dims, class T, class Acc, class Diff>
struct prefix_nd {
    using current = prefix_nd<dims, T, Acc, Diff>;
    using previous = prefix_nd<dims - 1, T, Acc, Diff>;

    std::vector<previous> p;
    Acc acc;
    Diff diff;
    static void combine(const current& to_add, current& output,
                        const Acc& acc) {
        int n = (int)to_add.p.size();
        for (int i = 0; i < n; ++i)
            previous::combine(to_add.p[i], output.p[i], acc);
    }
    prefix_nd() {}
    template <class U>
    prefix_nd(const std::vector<U>& a, const Acc& _acc, const Diff& _diff)
        : p(a.size()), acc(_acc), diff(_diff) {
        int n = (int)a.size();
        for (int i = 0; i < n; ++i) p[i] = previous(a[i], acc, diff);
        for (int i = 1; i < n; ++i) previous::combine(p[i - 1], p[i], acc);
    }
    template <typename... Args>
    T query(int l, int r, Args... args) {
        return (l == 0)
                   ? p[r - 1].query(args...)
                   : diff(p[r - 1].query(args...), p[l - 1].query(args...));
    }
};

template <class T, class Acc, class Diff>
struct prefix_nd<1, T, Acc, Diff> {
    using current = prefix_nd<1, T, Acc, Diff>;
    std::vector<T> p;
    Acc acc;
    Diff diff;
    static void combine(const current& to_add, current& output,
                        const Acc& acc) {
        int n = (int)output.p.size();
        for (int i = 0; i < n; ++i) output.p[i] = acc(to_add.p[i], output.p[i]);
    }
    prefix_nd() {}
    prefix_nd(const std::vector<T>& a, const Acc& _acc, const Diff& _diff)
        : p(a.size()), acc(_acc), diff(_diff) {
        std::partial_sum(std::begin(a), std::end(a), std::begin(p), acc);
    }
    // [l, r)
    T query(int l, int r) {
        return (l == 0) ? p[r - 1] : diff(p[r - 1], p[l - 1]);
    }
};

using namespace std;

int main() {
    auto acc = [](int a, int b) {
        return a + b;
    };
    auto diff = [](int a, int b) {
        return a - b;
    };

    vector b{vector{1, 2, 3, 4, 5}, vector{2, 4, 4, 5, 1},
             vector{3, 5, 7, 4, 2}};

    vector c{vector{2, 4, 2, 1, 6}, vector{1, 2, 3, 7, 4},
             vector{1, 3, 2, 5, 7}};

    vector d{vector{11, 21, 31, 41, 51}, vector{21, 41, 41, 51, 11},
             vector{31, 51, 71, 41, 21}};

    vector e{b, c, d};

    prefix_nd<3, int, decltype(acc), decltype(diff)> p3(e, acc, diff);
    cout << p3.query(0, 2, 1, 3, 2, 5) << '\n';
}

