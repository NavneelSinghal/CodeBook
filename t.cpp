#pragma GCC optimize("Ofast")
#pragma GCC target("avx")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>

using namespace __gnu_pbds;
using namespace __gnu_cxx;
using namespace std;

#define fastio                    \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

#define bitcount __builtin_popcountll
// for trailing 1s, do trailing0(n + 1)
#define leading0 __builtin_clzll
#define trailing0 __builtin_ctzll
#define isodd(n) (n & 1)
#define iseven(n) (!(n & 1))

#define del_rep(v) \
    sort(v.begin(), v.end());  \
    v.erase(unique(v.begin(), v.end()), v.end());
#define checkbit(n, b) ((n >> b) & 1)

// order_of_key(k) - number of elements e such that func(e, k) returns true,
// where func is less or less_equal find_by_order(k) - kth element in the set
// counting from 0

typedef tree<int, null_type, less<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_set;
typedef tree<int, null_type, less_equal<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_multiset;

const int INF = 1e9;
const long long LINF = INF * INF;
const double EPS = 1e-9;
const double PI = acosl(-1);
const int mod = 1e9 + 7;
const int maxn = 5e5 + 5;
const int maxa = 1e6 + 5;
const int logmax = 25;

void solve(int case_no) {}

signed main() {
    fastio;
    cout << setprecision(10) << fixed;
    int t = 1;
    for (int _t = 1; _t <= t; _t++) {
        solve(_t);
    }
    _flush_output();
    return 0;
}
