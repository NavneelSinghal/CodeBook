// https://atcoder.jp/contests/abc233/submissions/28155457
#ifndef LOCAL
    #pragma GCC optimize("O3,unroll-loops")
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")
// #pragma GCC target("avx2,sse4.2,bmi,bmi2,popcnt,lzcnt")
#endif

#include "bits/stdc++.h"

#ifdef DEBUG
    #include "includes/debug/debug.hpp"
#else
    #define debug(...) 0
#endif

using ll = int64_t;
using ld = long double;

using namespace std;

// 0-based indexing for API
// 1-based indexing for internals
template <typename T>
struct Fenwick {
    int n;
    vector<T> t;
    Fenwick(int n) : n(n), t(n + 1) {}
    // prefix sum [0, i)
    T query(int i) {
        T s = 0;
        while (i) {
            s += t[i];
            i -= i & (-i);
        }
        return s;
    }
    // range query [l, r)
    T query(int l, int r) { return query(r) - query(l); }
    // increase a[i] by v
    void update(int i, T v) {
        ++i;
        while (i <= n) {
            t[i] += v;
            i += i & (-i);
        }
    }

    // assumes f is monotonic - TT...TFF...F
    // returns the max r > 0 such that f(query([0, r)), r) is true
    // returns 0 if no such positive r exists
    template <class F>
    int max_right(F&& f) {
        // assert(f(0));
        T sum = 0;
        int pos = 0;
        for (int i = __lg(n); i >= 0; --i) {
            if (pos + (1 << i) <= n) {
                T s = sum + t[pos + (1 << i)];
                if (f(s, pos + (1 << i))) {
                    pos += (1 << i);
                    sum = s;
                }
            }
        }
        return pos;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // cout << setprecision(20) << fixed;
    int _tests = 1;
    // cin >> _tests;
    for (int _test = 1; _test <= _tests; ++_test) {
        // cout << "Case #" << _test << ": ";

        constexpr int N = 100000;
        vector<pair<int, int>> points;

        int n;
        cin >> n;
        for (int i = 0; i < n; ++i) {
            int x, y;
            cin >> x >> y;
            points.emplace_back(x + y, x - y + N);
        }

        int q;
        cin >> q;
        vector<tuple<int, int, int>> queries(q);
        for (auto& [x, y, k] : queries) {
            cin >> x >> y >> k;
            tie(x, y) = make_pair(x + y, x - y + N);
        }

        // function that takes in l and r arrays, and returns the value of the
        // predicate at mid for each query
        // essentially you're given a set of (query_id, mid for this query) and
        // you have to do something to check predicate for all of them at once
        auto work = [&](const vector<int>& l,
                        const vector<int>& r) -> vector<bool> {
            // make events for queries
            struct event {
                int x;
                int y;
                int y_l;    // for queries
                int y_r;    // for queries
                int type;   // -1 for start, 0 for add, 1 for end
                int index;  // query index
            };
            // insertion events
            vector<event> events;
            for (auto [x, y] : points)
                events.emplace_back(event{x, y, 0, 0, 0, 0});
            // query events
            for (int i = 0; i < q; ++i) {
                auto [x, y, k] = queries[i];
                int mid = (l[i] + r[i]) / 2;
                int y_l = max(0, y - mid);
                int y_r = min(y + mid, 2 * N) + 1;
                events.emplace_back(event{x - mid, y, y_l, y_r, -1, i});
                events.emplace_back(event{x + mid, y, y_l, y_r, +1, i});
            }
            sort(begin(events), end(events),
                 [](const event& a, const event& b) -> bool {
                     return tie(a.x, a.type, a.y) < tie(b.x, b.type, b.y);
                 });
            Fenwick<int> f(2 * N + 1);
            vector<int> cnt(q, 0);
            for (auto [x, y, y_l, y_r, type, index] : events) {
                if (type == 0)
                    f.update(y, 1);
                else
                    cnt[index] += type * f.query(y_l, y_r);
            }
            vector<bool> works(q);
            for (int i = 0; i < q; ++i)
                works[i] = (cnt[i] >= get<2>(queries[i]));
            return works;
        };

        // parallel binary search
        vector<int> L(q, -1), R(q, 2 * N + 1);
        while (true) {
            auto works = work(L, R);
            int completed = 0;
            for (int i = 0; i < q; ++i)
                if (R[i] - L[i] > 1)
                    (works[i] ? R[i] : L[i]) = (L[i] + R[i]) / 2;
                else
                    ++completed;
            if (completed == q) break;
        }
        for (int i = 0; i < q; ++i) cout << R[i] << '\n';
    }
}
