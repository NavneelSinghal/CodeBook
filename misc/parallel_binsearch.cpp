#ifndef LOCAL
    #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx,avx2,sse,sse2,sse3,sse4,popcnt,bmi,bmi2,lzcnt")
#endif

#include "bits/stdc++.h"

#ifdef DEBUG
    #include "includes/debug/debug.hpp"
#else
    #define debug(...) 0
#endif

using namespace std;

using ll = int64_t;
using ld = long double;

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // cout << setprecision(20) << fixed;
    int _tests = 1;
    // cin >> _tests;
    for (ll _test = 1; _test <= _tests; ++_test) {
        // cout << "Case #" << _test << ": ";

        int n, m;
        cin >> n >> m;
        vector<vector<int>> positions(n);
        for (int i = 0, x; i < m; ++i) {
            cin >> x;
            positions[x - 1].emplace_back(i);
        }
        vector<ll> to_gather(n);
        for (auto& meteors : to_gather) cin >> meteors;
        struct query {
            int l, r;
            ll a;
        };
        int k;
        cin >> k;
        vector<query> queries(k);
        for (auto& [l, r, a] : queries) {
            cin >> l >> r >> a;
            --l;
        }
        struct interval {
            int left, right;
        };

        // intervals[i] = (l, r) for binary searching for the ith state
        vector<interval> intervals(n, interval{0, k + 1});

        // states_at_cur[x] = states where at the next phase of binary search we
        // need to look at the first x (x = mid)
        vector<vector<int>> states_at_cur(k + 1);
        vector<vector<int>> states_at_temp(k + 1);

        // initially everything has range (0, k + 1) - note that at least 1 is
        // needed, if states want only 0, need to also include -1, and use an
        // offset vector instead
        for (int i = 0; i < n; ++i) states_at_cur[(k + 1) / 2].push_back(i);
        // how many are left to be processed
        int remaining = n;
        // segment tree
        vector<ll> segtree(2 * m + 1);
        while (true) {
            // segment tree range add
            auto update = [&](int l, int r, ll x) -> void {
                for (l += m, r += m; l < r; l >>= 1, r >>= 1) {
                    if (l & 1) segtree[l++] += x;
                    if (r & 1) segtree[--r] += x;
                }
            };
            // segment tree point query
            auto query = [&](int p) -> ll {
                ll x = 0;
                for (p += m; p > 0; p >>= 1) x += segtree[p];
                return x;
            };
            // in this phase, sweep through all updates and update the ranges of
            // each state at the same time
            for (int mid = 1; mid <= k; ++mid) {
                {
                    // do the update
                    auto [l, r, x] = queries[mid - 1];
                    if (l < r)
                        update(l, r, x);
                    else
                        update(l, m, x), update(0, r, x);
                }
                // for each state whose mid is mid
                for (auto state : states_at_cur[mid]) {
                    // check for gathered stuff
                    ll gathered = 0;
                    for (auto position : positions[state])
                        gathered += query(position),
                            gathered = min(gathered, ll(2e9));
                    // if we have gathered sufficient stuff, reduce r (else
                    // increase l)
                    auto& [l, r] = intervals[state];
                    if (gathered >= to_gather[state])
                        r = mid;
                    else
                        l = mid;
                    // if binary search has ended for this, just reduce
                    // remaining by 1, else put it in a new vector
                    if (l + 1 < r)
                        states_at_temp[(l + r) / 2].push_back(state);
                    else
                        remaining--;
                }
                states_at_cur[mid].clear();
            }
            swap(states_at_cur, states_at_temp);
            if (remaining == 0) break;
            fill(begin(segtree), end(segtree), ll(0));
        }
        for (int i = 0; i < n; ++i) {
            int ans = intervals[i].right;
            if (ans == k + 1)
                cout << "NIE\n";
            else
                cout << ans << '\n';
        }
    }
}

