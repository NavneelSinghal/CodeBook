#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
using ull = uint64_t;
using vi = vector<int>;
using pii = pair<int, int>;
using vpi = vector<pii>;
#define sz(v) int((v).size())
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define all(a) begin(a), end(a)
// Z FUNCTION; z[i] = lcp(s[:], s[i:]) except for z[0] = 0
vi Z(const string& S) {
    vi z(sz(S));
    int l = -1, r = -1;
    rep(i, 1, sz(S)) {
        z[i] = i >= r ? 0 : min(r - i, z[i - l]);
        while (i + z[i] < sz(S) && S[i + z[i]] == S[z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
/* SUFFIX ARRAY
 * sa[i] is the starting index of the suffix which is i-th in the sorted suffix
 * array. The returned vector is of size n+1, and sa[0] = n. The \texttt{lcp}
 * array contains longest common prefixes for neighbouring strings in the suffix
 * array: \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}. The input
 * string must not contain any zero bytes. Time: O(n \log n)
 */
struct SuffixArray {
    vi sa, lcp;
    SuffixArray(string& s, int lim = 256) {  // or basic_string<int>
        int n = sz(s) + 1, k = 0, a, b;
        vi x(all(s) + 1), y(n), ws(max(n, lim)), rank(n);
        sa = lcp = y, iota(all(sa), 0);
        for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
            p = j, iota(all(y), n - j);
            rep(i, 0, n) if (sa[i] >= j) y[p++] = sa[i] - j;
            fill(all(ws), 0);
            rep(i, 0, n) ws[x[i]]++;
            rep(i, 1, lim) ws[i] += ws[i - 1];
            for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
            swap(x, y), p = 1, x[sa[0]] = 0;
            rep(i, 1, n) a = sa[i - 1], b = sa[i], x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
        }
        rep(i, 1, n) rank[sa[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
            for (k &&k--, j = sa[rank[i] - 1]; s[i + k] == s[j + k]; k++)
                ;
    }
};
/* AHO CORASICK
 * Description: used for multiple pattern matching.
 * Initialize with AhoCorasick ac(patterns); the automaton start node will be at
 * index 0. find(word) returns for each position the index of the longest word
 * that ends there, or -1 if none. findAll($-$, word) finds all words(up to $N
 * \sqrt N$ many if no duplicate patterns) that start at each position(shortest
 * first). Duplicate patterns are allowed; empty patterns are not. To find the
 * longest words that start at each position, reverse all input. For large
 * alphabets, split each symbol into chunks, with sentinel bits for symbol
 * boundaries. Time: construction takes $O(26N)$, where $N =$ sum of length of
 * patterns. find(x) is $O(N) $, where N = length of x. findAll is $O(NM)$.
 */
struct AhoCorasick {
    enum { alpha = 26, first = 'A' };  // change this!
    struct Node {
        // (nmatches is optional)
        int back, next[alpha], start = -1, end = -1, nmatches = 0;
        Node(int v) { memset(next, v, sizeof(next)); }
    };
    vector<Node> N;
    vi backp;
    void insert(string& s, int j) {
        assert(!s.empty());
        int n = 0;
        for (char c : s) {
            int& m = N[n].next[c - first];
            if (m == -1) {
                n = m = sz(N);
                N.emplace_back(-1);
            } else
                n = m;
        }
        if (N[n].end == -1) N[n].start = j;
        backp.push_back(N[n].end);
        N[n].end = j;
        N[n].nmatches++;
    }
    AhoCorasick(vector<string>& pat) : N(1, -1) {
        rep(i, 0, sz(pat)) insert(pat[i], i);
        N[0].back = sz(N), N.emplace_back(0);
        queue<int> q;
        for (q.push(0); !q.empty(); q.pop()) {
            int n = q.front(), prev = N[n].back;
            rep(i, 0, alpha) {
                int &ed = N[n].next[i], y = N[prev].next[i];
                if (ed == -1) ed = y;
                else
                    N[ed].back = y, (N[ed].end == -1 ? N[ed].end : backp[N[ed].start]) = N[y].end, N[ed].nmatches += N[y].nmatches, q.push(ed);
            }
        }
    }
    vi find(string word) {
        int n = 0;
        vi res;                                                                 // ll count = 0;
        for (char c : word) n = N[n].next[c - first], res.push_back(N[n].end);  // count += N[n].nmatches;
        return res;
    }
    vector<vi> findAll(vector<string>& pat, string word) {
        vi r = find(word);
        vector<vi> res(sz(word));
        rep(i, 0, sz(word)) {
            int ind = r[i];
            while (ind != -1) res[i - sz(pat[ind]) + 1].push_back(ind), ind = backp[ind];
        }
        return res;
    }
};
// PREFIX FUNCTION
// pi[x] computes the length of the longest prefix of s that ends at x, other
// than s[0...x] itself (abacaba -> 0010123)
vi pi(const string& s) {
    vi p(sz(s));
    rep(i, 1, sz(s)) {
        int g = p[i - 1];
        while (g && s[i] != s[g]) g = p[g - 1];
        p[i] = g + (s[i] == s[g]);
    }
    return p;
}
vi match(const string& s, const string& pat) {
    vi p = pi(pat + '\0' + s), res;
    rep(i, sz(p) - sz(s), sz(p)) if (p[i] == sz(pat)) res.push_back(i - 2 * sz(pat));
    return res;
}
// MANACHER
// For each position in a string, computes p[0][i] = half length of longest even
// palindrome around pos i, p[1][i] = longest odd (half rounded down).
array<vi, 2> manacher(const string& s) {
    int n = sz(s);
    array<vi, 2> p = {vi(n + 1), vi(n)};
    rep(z, 0, 2) for (int i = 0, l = 0, r = 0; i < n; i++) {
        int t = r - i + !z;
        if (i < r) p[z][i] = min(t, p[z][l + t]);
        int L = i - p[z][i], R = i + p[z][i] - !z;
        while (L >= 1 && R + 1 < n && s[L - 1] == s[R + 1]) p[z][i]++, L--, R++;
        if (R > r) l = L, r = R;
    }
    return p;
}
// Each node represents an endpos-equivalent class
// Each class consists of some consecutive suffixes of its longest substring
// The suffix corresponding to minlen - 1 can be obtained by following the
// suffix link and looking at the longest substring in it
// Adjacency_Type: array<int, X> or map<Char_Type, int> where X is the size of
// the alphabet
template <class Char_Type, class Adjacency_Type>
struct suffix_automaton {
    // Begin States
    // len: length of the longest substring in the class, suffix link, minimum
    // value in the set endpos
    vector<int> len{0}, link{-1}, firstpos{-1}, is_clone{false};
    vector<Adjacency_Type> next{{}};
    // End States
    int new_state(int l, int sl, int fp, bool c, const Adjacency_Type& adj) {
        len.push_back(l);
        link.push_back(sl);
        firstpos.push_back(fp);
        is_clone.push_back(c);
        next.push_back(adj);
        return (int)len.size() - 1;
    }
    int last = 0;
    void extend(const vector<Char_Type>& s) {
        for (auto c : s) extend(c);
    }
    void extend(Char_Type c) {
        int cur = new_state(len[last] + 1, -1, len[last], false, {}), p = last;
        while (~p && !next[p][c]) next[p][c] = cur, p = link[p];
        if (!~p) link[cur] = 0;
        else {
            int q = next[p][c];
            if (len[p] + 1 == len[q]) link[cur] = q;
            else {
                int clone = new_state(len[p] + 1, link[q], firstpos[q], true, next[q]);
                while (~p && next[p][c] == q) next[p][c] = clone, p = link[p];
                link[q] = link[cur] = clone;
            }
        }
        last = cur;
        count.clear();
        inv_link.clear();
    }
    int size() const {  // # of states
        return (int)len.size();
    }
    int min_len(int u) const {  // length of the shortest string in the endpos equivalent class
        return u ? len[link[u]] + 1 : 0;
    }
    vector<int> terminals() const {
        vector<int> t;
        for (auto u = last; ~u; u = link[u]) t.push_back(u);
        return t;
    }
    vector<int> count;  // # of occurrences of substring represented by each states in
    // the string. Calling extend invalidates the array.
    vector<vector<int>> inv_link;
    void precalc_count() {
        int sz = (int)size();
        count.assign(sz, 0), inv_link.assign(sz, {});
        for (auto u = 1; u < sz; ++u) inv_link[link[u]].push_back(u);
        auto dfs = [&](auto self, int u) -> void {
            count[u] = !is_clone[u];
            for (auto v : inv_link[u]) {
                self(self, v);
                count[u] += count[v];
            }
        };
        dfs(dfs, 0);
    }
    vector<int> find_all_occurrences(int u) {
        assert(!count.empty() && u < size());
        vector<int> res;
        auto dfs = [&](auto self, int u) -> void {
            if (!is_clone[u]) res.push_back(firstpos[u]);
            for (auto v : inv_link[u]) self(self, v);
        };
        dfs(dfs, u);
        return res;
    }
};
// mint
template <class T, class D = void>
struct bigger_type {};
template <class T>
struct bigger_type<T, enable_if_t<sizeof(T) <= 4, void>> {
    using type = int64_t;
};
template <class T>
struct bigger_type<T, enable_if_t<4 < sizeof(T) && sizeof(T) <= 8, void>> {
    using type = __int128_t;
};
template <class T>
class Mint {
    using M = Mint;
    using Base = typename decay<decltype(T::value)>::type;
    using Bigger = typename bigger_type<Base>::type;
    static constexpr Base Zero = Base{0};
    static constexpr Base One = Base{1};

   public:
    Base mod() { return T::value; }
    Mint() : v_(Zero) {}
    M raw(Base v) { return *reinterpret_cast<M*>(&v); }
    template <class V>
    Mint(V v) : v_{Base((v %= mod()) < Zero ? v + mod() : v)} {}
    M& operator++() { return v_ = ++v_ == mod() ? Zero : v_, *this; }
    M& operator--() { return --(v_ ? v_ : v_ = mod()), *this; }
    M operator+() const { return *this; }
    M operator-() const { return raw(v_ ? mod() - v_ : Zero); }
    M& operator*=(M o) { return v_ = Bigger(v_) * o.v_ % mod(), *this; }
    M& operator/=(M o) {
        auto [inv, g] = extgcd(o.v_, mod());
        assert(g == One);
        return *this *= inv;
    }
    M& operator+=(M o) { return v_ = Base(v_ += o.v_ - mod()) < Zero ? v_ + mod() : v_, *this; }
    M& operator-=(M o) { return v_ = Base(v_ -= o.v_) < Zero ? v_ + mod() : v_, *this; }
    friend M operator++(M& a, int) { return exchange(a, ++M(a)); }
    friend M operator--(M& a, int) { return exchange(a, --M(a)); }
    friend M operator*(M a, M b) { return a *= b; }
    friend M operator/(M a, M b) { return a /= b; }
    friend M operator+(M a, M b) { return a += b; }
    friend M operator-(M a, M b) { return a -= b; }
    friend istream& operator>>(istream& is, M& x) {
        int64_t v;
        return is >> v, x = v, is;
    }
    friend ostream& operator<<(ostream& os, M x) { return os << x.v_; }
    friend bool operator==(M a, M b) { return a.v_ == b.v_; }
    friend bool operator!=(M a, M b) { return a.v_ != b.v_; }

   private:
    static array<Base, 2> extgcd(Base a, Base b) {
        array x{One, Zero};
        while (b) swap(x[0] -= a / b * x[1], x[1]), swap(a %= b, b);
        return {x[0], a};
    }
    Base v_;
};
// constant mod
constexpr int md = static_cast<int>(1e9 + 7);
using mint1 = Mint<integral_constant<decay<decltype(md)>::type, md>>;
// variable mod
using ModType = int;
struct VarMod {
    inline static ModType value;
    using type = ModType;
};
auto& mod_var = VarMod::value;
using mint2 = Mint<VarMod>;
// PBDS
#include <bits/extc++.h> /** keep-include */
// void example() {
//     Tree<int> t, t2; t.insert(8);
//     auto it = t.insert(10).first;
//     assert(it == t.lower_bound(9));
//     assert(t.order_of_key(10) == 1);
//     assert(t.order_of_key(11) == 2);
//     assert(*t.find_by_order(0) == 8);
//     t.join(t2); // assuming T < T2 or T > T2, merge t2 into t
// }
struct chash {  // large odd number for C
    const uint64_t C = ll(4e18 * acos(0)) | 71;
    ll operator()(ll x) const { return __builtin_bswap64(x * C); }
};
__gnu_pbds::gp_hash_table<ll, int, chash> h({}, {}, {}, {}, {1 << 16});
template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<K, V, Comp, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;
// Supports
//  auto iterator = order_statistic_set().find_by_order(idx); // (0-indexed)
//  int num_strictly_smaller = order_statistic_set().order_of_key(key);
template <typename T>
using min_heap = priority_queue<T, vector<T>, greater<T>>;
template <class T, class G>
auto dijkstra(const G& g, int s) {
    vector d(g.size(), numeric_limits<T>::max());
    vector prv(g.size(), -1);
    min_heap<pair<T, int>> rh;
    rh.emplace(d[s] = 0, s);
    while (!rh.empty()) {
        auto [dv, v] = rh.top();
        rh.pop();
        if (dv != d[v]) continue;
        for (auto&& [to, w] : g[v])
            if (d[to] > dv + w) d[to] = dv + w, rh.emplace(d[to], to), prv[to] = v;
    }
    return make_pair(d, prv);
}
/* TOPOSORT
 * If there are cycles, the returned list will have size smaller than n - nodes
 * reachable from cycles will not be returned.
 */
vi topoSort(const vector<vi>& gr) {
    vi indeg(sz(gr)), ret;
    for (auto& li : gr)
        for (int x : li) indeg[x]++;
    queue<int> q;  // use priority_queue for lexic. largest ans.
    rep(i, 0, sz(gr)) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int i = q.front();  // top() for priority queue
        ret.push_back(i);
        q.pop();
        for (int x : gr[i])
            if (--indeg[x] == 0) q.push(x);
    }
    return ret;
}
/* BELLMAN FORD
 * Description: Calculates shortest paths from $s$ in a graph that might have
 * negative edge weights. Unreachable nodes get dist = inf; nodes reachable
 * through negative-weight cycles get dist = -inf. Assumes $V^2 \max |w_i| <
 * \tilde{} 2^{63}$.
 */
const ll inf = LLONG_MAX;
struct Ed {
    int a, b, w;
    int s() { return a < b ? a : -a; }
};
struct Node {
    ll dist = inf;
    int prv = -1;
};
void bellmanFord(vector<Node>& nodes, vector<Ed>& eds, int s) {
    nodes[s].dist = 0;
    sort(all(eds), [](Ed a, Ed b) { return a.s() < b.s(); });
    int lim = sz(nodes) / 2 + 2;  // /3+100 with shuffled vertices
    rep(i, 0, lim) for (Ed ed : eds) {
        Node cur = nodes[ed.a], &dest = nodes[ed.b];
        if (abs(cur.dist) == inf) continue;
        ll d = cur.dist + ed.w;
        if (d < dest.dist) dest.prv = ed.a, dest.dist = (i < lim - 1 ? d : -inf);
    }
    rep(i, 0, lim) for (Ed e : eds) if (nodes[e.a].dist == -inf) nodes[e.b].dist = -inf;
}
// faster hash map
typedef uint64_t ull;
template <class T>
struct HashMap {
    int b;
    vector<pair<ull, T>> v;
    HashMap(int b) : b(b), v(1 << b) {}
    T& operator[](ull x) {
        ull y = x >> (64 - b), m = (1 << b) - 1;
        while (v[y].first && v[y].first != x) ++y &= m;
        v[y].first = x;
        return v[y].second;
    }
};
template <class T, class Op>
struct RQ {  // disjoint sparse table
    vector<vector<T>> s;
    Op op;
    RQ(const vector<T>& a, Op op_) : s{{a}}, op{op_} {
        for (int j = 2; j < sz(a); j *= 2)
            for (int k = (s.push_back(a), j); k < sz(a); k += 2 * j) {
                auto r = rend(s.back()) - k;
                auto f = r.base();
                partial_sum(f, f + min(j, sz(a) - k), f, op);
                partial_sum(r, r + j, r, [&](T x, T y) { return op(y, x); });
            }
    }
    T query(int l, int r) {  // [l, r), needs nonempty
        if (l == --r) return s[0][l];
        int b = __lg(l ^ r);
        return op(s[b][l], s[b][r]);
    }
};
struct LCA {
    int T = 0;
    vi time, path, ret;
    static constexpr auto f = [](int x, int y) {
        return min(x, y);
    };
    RMQ<int, decltype(f)> rmq;
    LCA(vector<vi>& C) : time(sz(C)), rmq((dfs(C, 0, -1), ret), f) {}
    void dfs(vector<vi>& C, int v, int par) {
        time[v] = T++;
        for (int y : C[v])
            if (y != par) {
                path.push_back(v), ret.push_back(time[v]);
                dfs(C, y, v);
            }
    }
    int lca(int a, int b) {
        if (a == b) return a;
        tie(a, b) = minmax(time[a], time[b]);
        return path[rmq.query(a, b)];
    }
    // dist(a, b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};
/* VIRTUAL TREE:
 * Description: Given a rooted tree and a subset S of nodes, compute the minimal
 * subtree that contains all the nodes by adding all (at most $|S|-1$)
 * pairwise LCA's and compressing edges.
 * Returns a list of (par, orig\_index) representing a tree rooted at 0.
 * The root points to itself.
 * Time: $O(|S| \log |S|)$
 * Status: Tested at CodeForces
 */
vpi compressTree(LCA& lca, const vi& subset) {
    static vi rev;
    rev.resize(sz(lca.time));
    vi li = subset, &T = lca.time;
    auto cmp = [&](int a, int b) {
        return T[a] < T[b];
    };
    sort(all(li), cmp);
    int m = sz(li) - 1;
    rep(i, 0, m) {
        int a = li[i], b = li[i + 1];
        li.push_back(lca.lca(a, b));
    }
    sort(all(li), cmp);
    li.erase(unique(all(li)), li.end());
    rep(i, 0, sz(li)) rev[li[i]] = i;
    vpi ret = {pii(0, li[0])};
    rep(i, 0, sz(li) - 1) {
        int a = li[i], b = li[i + 1];
        ret.emplace_back(rev[lca.lca(a, b)], b);
    }
    return ret;
}
/*
 * Description: Calculate power of two jumps in a tree,
 * to support fast upward jumps and LCAs.
 * Assumes the root node points to itself.
 */
vector<vi> treeJump(vi& P) {
    int on = 1, d = 1;
    while (on < sz(P)) on *= 2, d++;
    vector<vi> jmp(d, P);
    rep(i, 1, d) rep(j, 0, sz(P)) jmp[i][j] = jmp[i - 1][jmp[i - 1][j]];
    return jmp;
}
int jmp(vector<vi>& tbl, int nod, int steps) {
    rep(i, 0, sz(tbl)) if (steps & (1 << i)) nod = tbl[i][nod];
    return nod;
}
int lca(vector<vi>& tbl, vi& depth, int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);
    a = jmp(tbl, a, depth[a] - depth[b]);
    if (a == b) return a;
    for (int i = sz(tbl); i--;) {
        int c = tbl[i][a], d = tbl[i][b];
        if (c != d) a = c, b = d;
    }
    return tbl[0][a];
}
/* DINIC
 * Description: Flow algorithm with complexity $O(VE\log U)$ where $U = \max
 |\text{cap}|$.
 * $O(\min(E^{1/2}, V^{2/3})E)$ if $U = 1$; $O(\sqrt{V}E)$ for bipartite
 matching.
 */
struct Dinic {
    struct Edge {
        int to, rev;
        ll c, oc;
        ll flow() { return max(oc - c, ll(0)); }  // if you need flows
    };
    vi lvl, ptr, q;
    vector<vector<Edge>> adj;
    Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
    void addEdge(int a, int b, ll c, ll rcap = 0) {
        adj[a].push_back({b, sz(adj[b]), c, c});
        adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
    }
    ll dfs(int v, int t, ll f) {
        if (v == t || !f) return f;
        for (int& i = ptr[v]; i < sz(adj[v]); i++) {
            Edge& e = adj[v][i];
            if (lvl[e.to] == lvl[v] + 1)
                if (ll p = dfs(e.to, t, min(f, e.c))) {
                    e.c -= p, adj[e.to][e.rev].c += p;
                    return p;
                }
        }
        return 0;
    }
    ll calc(int s, int t) {
        ll flow = 0;
        q[0] = s;
        rep(L, 0, 31) do {  // 'int L=30' maybe faster for random data
            lvl = ptr = vi(sz(q));
            int qi = 0, qe = lvl[s] = 1;
            while (qi < qe && !lvl[t]) {
                int v = q[qi++];
                for (Edge e : adj[v])
                    if (!lvl[e.to] && e.c >> (30 - L)) q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
            }
            while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
        }
        while (lvl[t])
            ;
        return flow;
    }
    bool leftOfMinCut(int a) { return lvl[a] != 0; }
};
/* EULER WALK
 * Description: Eulerian undirected/directed path/cycle algorithm.
 * Input should be a vector of (dest, global edge index), where
 * for undirected graphs, forward/backward edges have the same index.
 * Returns a list of nodes in the Eulerian path/cycle with src at both start and
 * end, or empty list if no cycle/path exists. To get edge indices back, add
 * .second to s and ret. Time: O(V + E)
 */
vi eulerWalk(vector<vector<pii>>& gr, int nedges, int src = 0) {
    int n = sz(gr);
    vi D(n), its(n), eu(nedges), ret, s = {src};
    D[src]++;  // to allow Euler paths, not just cycles
    while (!s.empty()) {
        int x = s.back(), y, e, &it = its[x], end = sz(gr[x]);
        if (it == end) {
            ret.push_back(x), s.pop_back();
            continue;
        }
        tie(y, e) = gr[x][it++];
        if (!eu[e]) D[x]--, D[y]++, eu[e] = 1, s.push_back(y);
    }
    for (int x : D)
        if (x < 0 || sz(ret) != nedges + 1) return {};
    return {ret.rbegin(), ret.rend()};
}
// FLOYD WARSHALL
const ll inf = 1LL << 62;
void floydWarshall(vector<vector<ll>>& m) {
    int n = sz(m);
    rep(i, 0, n) m[i][i] = min(m[i][i], 0LL);
    rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) if (m[i][k] != inf && m[k][j] != inf) {
        auto newDist = max(m[i][k] + m[k][j], -inf);
        m[i][j] = min(m[i][j], newDist);
    }
    rep(k, 0, n) if (m[k][k] < 0) rep(i, 0, n) rep(j, 0, n) if (m[i][k] != inf && m[k][j] != inf) m[i][j] = -inf;
}
// LAZY SEGTREE
// rather than writing new Node each time, use a global buffer of a large number
// of nodes (or a deque)
const int inf = 1e9;
struct Node {
    Node *l = 0, *r = 0;
    int lo, hi, mset = inf, madd = 0, val = -inf;
    Node(int lo, int hi) : lo(lo), hi(hi) {}  // Large interval of -inf
    Node(vi& v, int lo, int hi) : lo(lo), hi(hi) {
        if (lo + 1 < hi) {
            int mid = lo + (hi - lo) / 2;
            l = new Node(v, lo, mid), r = new Node(v, mid, hi);
            val = max(l->val, r->val);
        } else
            val = v[lo];
    }
    int query(int L, int R) {
        if (R <= lo || hi <= L) return -inf;
        if (L <= lo && hi <= R) return val;
        push();
        return max(l->query(L, R), r->query(L, R));
    }
    void set(int L, int R, int x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) mset = val = x, madd = 0;
        else
            push(), l->set(L, R, x), r->set(L, R, x), val = max(l->val, r->val);
    }
    void add(int L, int R, int x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) {
            (mset != inf) ? (mset += x) : (madd += x);
            val += x;
        } else {
            push(), l->add(L, R, x), r->add(L, R, x);
            val = max(l->val, r->val);
        }
    }
    void push() {
        if (!l) {
            int mid = lo + (hi - lo) / 2;
            l = new Node(lo, mid), r = new Node(mid, hi);
        }
        if (mset != inf) l->set(lo, hi, mset), r->set(lo, hi, mset), mset = inf;
        else if (madd)
            l->add(lo, hi, madd), r->add(lo, hi, madd), madd = 0;
    }
};
/* HLD
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Code does additive modifications and max queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * Takes as input the full adjacency list. VALS\_EDGES being true means that
 * values are stored in the edges, as opposed to the nodes. All values
 * initialized to the segtree default. Root must be 0.
 * Time: O((\log N)^2)
 */
template <bool VALS_EDGES>
struct HLD {
    int N, tim = 0;
    vector<vi> adj;
    vi par, siz, depth, rt, pos;
    Node* tree;
    // clang-format off
    HLD(vector<vi> adj_)
        : N(sz(adj_)), adj(adj_), par(N, -1), siz(N, 1), depth(N),
        rt(N), pos(N), tree(new Node(0, N)) { dfsSz(0), dfsHld(0); }
    // clang-format on
    void dfsSz(int v) {
        if (par[v] != -1) adj[v].erase(find(all(adj[v]), par[v]));
        for (int& u : adj[v]) {
            par[u] = v, depth[u] = depth[v] + 1;
            dfsSz(u);
            siz[v] += siz[u];
            if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
        }
    }
    void dfsHld(int v) {
        pos[v] = tim++;
        for (int u : adj[v]) rt[u] = (u == adj[v][0] ? rt[v] : u), dfsHld(u);
    }
    template <class B>
    void process(int u, int v, B op) {
        for (; rt[u] != rt[v]; v = par[rt[v]]) {
            if (depth[rt[u]] > depth[rt[v]]) swap(u, v);
            op(pos[rt[v]], pos[v] + 1);
        }
        if (depth[u] > depth[v]) swap(u, v);
        op(pos[u] + VALS_EDGES, pos[v] + 1);
    }
    void modifyPath(int u, int v, int val) {
        process(u, v, [&](int l, int r) { tree->add(l, r, val); });
    }
    int queryPath(int u, int v) {  // Modify depending on problem
        int res = -1e9;
        process(u, v, [&](int l, int r) { res = max(res, tree->query(l, r)); });
        return res;
    }
    int querySubtree(int v) {  // modifySubtree is similar
        return tree->query(pos[v] + VALS_EDGES, pos[v] + siz[v]);
    }
    int lca(int u, int v) {
        for (; rt[u] != rt[v]; v = par[rt[v]])
            if (depth[rt[u]] > depth[rt[v]]) swap(u, v);
        return depth[u] < depth[v] ? u : v;
    }
};
/* HOPCROFT KARP
 * Description: Fast bipartite matching algorithm. Graph $g$ should be a list
 * of neighbors of the left partition, and $btoa$ should be a vector full of
 * -1's of the same size as the right partition. Returns the size of
 * the matching. $btoa[i]$ will be the match for vertex $i$ on the right side,
 * or $-1$ if it's not matched.
 * Usage: vi btoa(m, -1); hopcroftKarp(g, btoa);
 */
bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
    if (A[a] != L) return 0;
    A[a] = -1;
    for (int b : g[a])
        if (B[b] == L + 1) {
            B[b] = 0;
            if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B)) return btoa[b] = a, 1;
        }
    return 0;
}
int hopcroftKarp(vector<vi>& g, vi& btoa) {
    int res = 0;
    vi A(g.size()), B(btoa.size()), cur, next;
    for (;;) {
        fill(all(A), 0), fill(all(B), 0);
        /// Find the starting nodes for BFS (i.e. layer 0).
        cur.clear();
        for (int a : btoa)
            if (a != -1) A[a] = -1;
        rep(a, 0, sz(g)) if (A[a] == 0) cur.push_back(a);
        /// Find all layers using bfs.
        for (int lay = 1;; lay++) {
            bool islast = 0;
            next.clear();
            for (int a : cur)
                for (int b : g[a])
                    if (btoa[b] == -1) B[b] = lay, islast = 1;
                    else if (btoa[b] != a && !B[b])
                        B[b] = lay, next.push_back(btoa[b]);
            if (islast) break;
            if (next.empty()) return res;
            for (int a : next) A[a] = lay;
            cur.swap(next);
        }
        /// Use DFS to scan for augmenting paths.
        rep(a, 0, sz(g)) res += dfs(a, 0, g, btoa, A, B);
    }
}
// KUHN
bool find(int j, vector<vi>& g, vi& btoa, vi& vis) {
    if (btoa[j] == -1) return 1;
    vis[j] = 1;
    int di = btoa[j];
    for (int e : g[di])
        if (!vis[e] && find(e, g, btoa, vis)) btoa[e] = di, return 1;
    return 0;
}
int dfsMatching(vector<vi>& g, vi& btoa) {
    vi vis;
    rep(i, 0, sz(g)) {
        vis.assign(sz(btoa), 0);
        for (int j : g[i])
            if (find(j, g, btoa, vis)) {
                btoa[j] = i;
                break;
            }
    }
    return sz(btoa) - (int)count(all(btoa), -1);
}
/*
 * Description: Finds a minimum vertex cover in a bipartite graph.
 *  The size is the same as the size of a maximum matching, and
 *  the complement is a maximum independent set.
 */
vi cover(vector<vi>& g, int n, int m) {
    vi match(m, -1);
    int res = dfsMatching(g, match);
    vector<bool> lfound(n, true), seen(m);
    for (int it : match)
        if (it != -1) lfound[it] = false;
    vi q, cover;
    rep(i, 0, n) if (lfound[i]) q.push_back(i);
    while (!q.empty()) {
        int i = q.back();
        q.pop_back();
        lfound[i] = 1;
        for (int e : g[i])
            if (!seen[e] && match[e] != -1) {
                seen[e] = true;
                q.push_back(match[e]);
            }
    }
    rep(i, 0, n) if (!lfound[i]) cover.push_back(i);
    rep(i, 0, m) if (seen[i]) cover.push_back(n + i);
    assert(sz(cover) == res);
    return cover;
}
/* SCC
 * Description: Finds strongly connected components in a
 * directed graph. If vertices $u, v$ belong to the same component,
 * we can reach $u$ from $v$ and vice versa.
 * Usage: scc(graph, [\&](vi\& v) { ... }) visits all components
 * in reverse topological order. comp[i] holds the component
 * index of a node (a component only has edges to components with
 * lower index). ncomps will contain the number of components.
 * Time: O(E + V)
 */
template <class G, class F>
void scc(G& g, F f) {
    int n = sz(g);
    vi val(n), comp(n, -1), z, cont;
    int Time = 0, ncomps = 0;
    auto dfs = [&](auto self, int j) -> int {
        int low = val[j] = ++Time, x;
        z.push_back(j);
        for (auto e : g[j])
            if (comp[e] < 0) low = min(low, val[e] ?: self(self, e));
        if (low == val[j]) {
            do {
                x = z.back(), z.pop_back(), comp[x] = ncomps, cont.push_back(x);
            } while (x != j);
            f(cont), cont.clear(), ncomps++;
        }
    };
    rep(i, 0, n) if (comp[i] < 0) dfs(i, g, f);
}
// SIEVE - compute multiplicative function
template <int n = 1'000'000>
struct sieve {
    vector<int> primes, spf;
    vector<ll> func;
    vector<char> is_prime;
    sieve() {
        is_prime.assign(n + 1, true), spf.resize(n + 1), func.resize(n + 1);
        is_prime[0] = is_prime[1] = false, func[1] = 1;  // either 1 or 0
        for (int i = 2; i <= n; ++i) {
            if (is_prime[i])
                // handle the case of primes by updating func[i]
                primes.push_back(i), spf[i] = i;
            int spfi = spf[i];
            for (auto p : primes) {
                int k = i * p;
                if (k > n) break;
                is_prime[k] = false;
                // p <= spfi
                spf[k] = p;
                if (spfi == p) {
                    // p divides i, update func
                } else {
                    // p and i are coprime, update func
                }
            }
        }
    }
};
template <class T, class U, class F>
T pwr(T a, U n, F&& f, T base = 1) {
    while (n) {
        if (n & 1) base = f(base, a);
        if (n >>= 1) a = f(a, a);
    }
    return base;
}
// miller rabin
bool isPrime(ull n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}, s = __builtin_ctzll(n - 1), d = n >> s;
    for (ull a : A) {  // ^ count trailing zeroes
        ull p = pwr(a % n, d, [&](ull x, ull y) { return x * y % n; }), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) p = ull(p) * p % n;
        if (p != n - 1 && i != s) return 0;
    }
    return 1;
}
// factor
ull pollard(ull n) {
    auto modmul = [](ull x, ull y, ull n) {
        return x * y % n;
    };
    auto f = [=](ull x) {
        return modmul(x, x, n) + 1;
    };
    ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
    while (t++ % 40 || __gcd(prd, n) == 1) {
        if (x == y) x = ++i, y = f(x);
        if ((q = modmul(prd, max(x, y) - min(x, y), n))) prd = q;
        x = f(x), y = f(f(y));
    }
    return __gcd(prd, n);
}
vector<ull> factor(ull n) {
    if (n == 1) return {};
    if (isPrime(n)) return {n};
    ull x = pollard(n);
    auto l = factor(x), r = factor(n / x);
    l.insert(l.end(), all(r));
    return l;
}
typedef double d;  // for N ~ 1e7; long double for N ~ 1e9
pair<ll, ll> approximate(d x, ll N) {
    ll LP = 0, LQ = 1, P = 1, Q = 0, inf = LLONG_MAX;
    d y = x;
    for (;;) {
        ll lim = min(P ? (N - LP) / P : inf, Q ? (N - LQ) / Q : inf), a = (ll)floor(y), b = min(a, lim), NP = b * P + LP, NQ = b * Q + LQ;
        if (a > b) {
            // If b > a/2, we have a semi-convergent that gives us a
            // better approximation; if b = a/2, we *may* have one.
            // Return {P, Q} here for a more canonical approximation.
            return (abs(x - (d)NP / (d)NQ) < abs(x - (d)P / (d)Q)) ? make_pair(NP, NQ) : make_pair(P, Q);
        }
        if (abs(y = 1 / (y - (d)a)) > 3 * N) return {NP, NQ};
        LP = P, P = NP, LQ = Q, Q = NQ;
    }
}
// crt
ll euclid(ll a, ll b, ll& x, ll& y) {
    if (!b) return x = 1, y = 0, a;
    ll d = euclid(b, a % b, y, x);
    return y -= a / b * x, d;
}
ll crt(ll a, ll m, ll b, ll n) {
    if (n > m) swap(a, b), swap(m, n);
    ll x, y, g = euclid(m, n, x, y);
    assert((a - b) % g == 0);  // else no solution
    x = (b - a) % n * x % n / g * m + a;
    return x < 0 ? x + m * n / g : x;
}
typedef complex<double> C;
typedef vector<double> vd;
void fft(vector<C>& a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);  // (^ 10% faster if double)
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n);
        rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        rep(i, k, 2 * k) rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
    vi rev(n);
    rep(i, 0, n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i, 0, n) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k) rep(j, 0, k) {
                auto x = (double*)&rt[j + k], y = (double*)&a[i + j + k];
                C z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}
vd conv(const vd& a, const vd& b) {
    if (a.empty() || b.empty()) return {};
    vd res(sz(a) + sz(b) - 1);
    int L = 32 - __builtin_clz(sz(res)), n = 1 << L;
    vector<C> in(n), out(n);
    copy(all(a), begin(in));
    rep(i, 0, sz(b)) in[i].imag(b[i]);
    fft(in);
    for (C& x : in) x *= x;
    rep(i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    rep(i, 0, sz(res)) res[i] = imag(out[i]) / (4 * n);
    return res;
}
typedef vector<ll> vl;
template <int M>
vl convMod(const vl& a, const vl& b) {
    if (a.empty() || b.empty()) return {};
    vl res(sz(a) + sz(b) - 1);
    int B = 32 - __builtin_clz(sz(res)), n = 1 << B, cut = int(sqrt(M));
    vector<C> L(n), R(n), outs(n), outl(n);
    rep(i, 0, sz(a)) L[i] = C((int)a[i] / cut, (int)a[i] % cut);
    rep(i, 0, sz(b)) R[i] = C((int)b[i] / cut, (int)b[i] % cut);
    fft(L), fft(R);
    rep(i, 0, n) {
        int j = -i & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
    }
    fft(outl), fft(outs);
    rep(i, 0, sz(res)) {
        ll av = ll(real(outl[i]) + .5), cv = ll(imag(outs[i]) + .5);
        ll bv = ll(imag(outl[i]) + .5) + ll(real(outs[i]) + .5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}
const ll mod = (119 << 23) + 1;  // = 998244353
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.
using mint = Mint<integral_constant<int, md>>;
mint root = 62;
typedef vector<mint> vm;
void ntt(vm& a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vm rt(2, 1);
    for (static int k = 2, s = 2; k < n; k *= 2, s++) {
        rt.resize(n);
        mint z[] = {1, pwr(root, mod >> s, multiplies<mint>{})};
        rep(i, k, 2 * k) rt[i] = rt[i / 2] * z[i & 1];
    }
    vi rev(n);
    rep(i, 0, n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i, 0, n) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k) rep(j, 0, k) {
                auto z = rt[j + k] * a[i + j + k], &ai = a[i + j];
                a[i + j + k] = ai - z, ai += z;
            }
}
vm conv(const vm& a, const vm& b) {
    if (a.empty() || b.empty()) return {};
    int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    auto inv = 1 / mint(n);
    vm L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    ntt(L), ntt(R);
    rep(i, 0, n) out[-i & (n - 1)] = L[i] * R[i] * inv;
    ntt(out);
    return {out.begin(), out.begin() + s};
}
// DSU with rollbacks
struct RollbackUF {
    vi e;
    vector<pii> st;
    RollbackUF(int n) : e(n, -1) {}
    int size(int x) { return -e[find(x)]; }
    int find(int x) { return e[x] < 0 ? x : find(e[x]); }
    int time() { return sz(st); }
    void rollback(int t) {
        for (int i = time(); i-- > t;) e[st[i].first] = st[i].second;
        st.resize(t);
    }
    bool join(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (e[a] > e[b]) swap(a, b);
        st.push_back({a, e[a]}), st.push_back({b, e[b]});
        e[a] += e[b], e[b] = a;
        return true;
    }
};
struct Fenwick {
    vector<ll> s;
    Fenwick(int n) : s(n) {}
    void update(int pos, ll dif) {  // a[pos] += dif
        for (; pos < sz(s); pos |= pos + 1) s[pos] += dif;
    }
    ll query(int pos) {  // sum of values in [0, pos)
        ll res = 0;
        for (; pos > 0; pos &= pos - 1) res += s[pos - 1];
        return res;
    }
    int lower_bound(ll sum) {  // min pos st sum of [0, pos] >= sum
        // Returns n if no sum is >= sum, or -1 if empty sum is.
        if (sum <= 0) return -1;
        int pos = 0;
        for (int pw = 1 << 25; pw; pw >>= 1)
            if (pos + pw <= sz(s) && s[pos + pw - 1] < sum) pos += pw, sum -= s[pos - 1];
        return pos;
    }
};
// 2d
struct Fenwick2D {
    vector<vi> ys;
    vector<Fenwick> ft;
    Fenwick2D(int limx) : ys(limx) {}
    void fakeUpdate(int x, int y) {  // call on all queries before init
        for (; x < sz(ys); x |= x + 1) ys[x].push_back(y);
    }
    void init() {
        for (vi& v : ys) sort(all(v)), ft.emplace_back(sz(v));
    }
    int ind(int x, int y) { return (int)(lower_bound(all(ys[x]), y) - ys[x].begin()); }
    void update(int x, int y, ll dif) {
        for (; x < sz(ys); x |= x + 1) ft[x].update(ind(x, y), dif);
    }
    ll query(int x, int y) {
        ll sum = 0;
        for (; x; x &= x - 1) sum += ft[x - 1].query(ind(x - 1, y));
        return sum;
    }
};
struct Line {
    mutable ll k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};
struct LineContainer : multiset<Line, less<>> {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    static const ll inf = LLONG_MAX;
    ll div(ll a, ll b) {  // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        x->p = (x->k == y->k) ? (x->m > y->m ? inf : -inf) : div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {  // add kx + m
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    ll query(ll x) {  // query max at x
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};
// MO
void add(int ind, int end) {}  // add a[ind] (end = 0 or 1)
void del(int ind, int end) {}  // remove a[ind]
int calc() {}                  // compute current answer
vi mo(vector<pii> Q) {
    int L = 0, R = 0, blk = 350;  // ~N/sqrt(Q)
    vi s(sz(Q)), res = s;
#define K(x) pii(x.first / blk, x.second ^ -(x.first / blk & 1))
    iota(all(s), 0);
    sort(all(s), [&](int s, int t) { return K(Q[s]) < K(Q[t]); });
    for (int qi : s) {
        pii q = Q[qi];
        while (L > q.first) add(--L, 0);
        while (R < q.second) add(R++, 1);
        while (L < q.first) del(L++, 0);
        while (R > q.second) del(--R, 1);
        res[qi] = calc();
    }
    return res;
}
vi moTree(vector<array<int, 2>> Q, vector<vi>& ed, int root = 0) {
    int N = sz(ed), pos[2] = {}, blk = 350;  // ~N/sqrt(Q)
    vi s(sz(Q)), res = s, I(N), L(N), R(N), in(N), par(N);
    add(0, 0), in[0] = 1;
    auto dfs = [&](int x, int p, int dep, auto& f) -> void {
        par[x] = p;
        L[x] = N;
        if (dep) I[x] = N++;
        for (int y : ed[x])
            if (y != p) f(y, x, !dep, f);
        if (!dep) I[x] = N++;
        R[x] = N;
    };
    dfs(root, -1, 0, dfs);
#define K(x) pii(I[x[0]] / blk, I[x[1]] ^ -(I[x[0]] / blk & 1))
    iota(all(s), 0);
    sort(all(s), [&](int s, int t) { return K(Q[s]) < K(Q[t]); });
    for (int qi : s) rep(end, 0, 2) {
            int &a = pos[end], b = Q[qi][end], i = 0;
            auto step = [&](int c) {
                if (in[c]) del(a, end), in[a] = 0;
                else
                    add(c, end), in[c] = 0;
                a = c;
            };
            while (!(L[b] <= L[a] && R[a] <= R[b])) I[i++] = b, b = par[b];
            while (a != b) step(par[a]);
            while (i--) step(I[i]);
            if (end) res[qi] = calc();
        }
    return res;
}
// normal segtree
struct Tree {
    typedef int T;
    static constexpr T unit = INT_MIN;
    T f(T a, T b) { return max(a, b); }  // (any associative fn)
    vector<T> s;
    int n;
    Tree(int n = 0, T def = unit) : s(2 * n, def), n(n) {}
    void update(int pos, T val) {
        for (s[pos += n] = val; pos /= 2;) s[pos] = f(s[pos * 2], s[pos * 2 + 1]);
    }
    T query(int b, int e) {  // query [b, e)
        T ra = unit, rb = unit;
        for (b += n, e += n; b < e; b /= 2, e /= 2) {
            if (b % 2) ra = f(ra, s[b++]);
            if (e % 2) rb = f(s[--e], rb);
        }
        return f(ra, rb);
    }
};
// treap
struct Node {
    Node *l = 0, *r = 0;
    int val, y, c = 1;
    Node(int val) : val(val), y(rand()) {}
    void recalc();
};
int cnt(Node* n) {
    return n ? n->c : 0;
}
void Node::recalc() {
    c = cnt(l) + cnt(r) + 1;
}
template <class F>
void each(Node* n, F f) {
    if (n) each(n->l, f), f(n->val), each(n->r, f);
}
pair<Node*, Node*> split(Node* n, int k) {
    if (!n) return {};
    if (cnt(n->l) >= k) {  // "n->val >= k" for lower_bound(k)
        auto pa = split(n->l, k);
        n->l = pa.second;
        n->recalc();
        return {pa.first, n};
    } else {
        auto pa = split(n->r, k - cnt(n->l) - 1);  // and just "k"
        n->r = pa.first;
        n->recalc();
        return {n, pa.second};
    }
}
Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->y > r->y) {
        l->r = merge(l->r, r), l->recalc();
        return l;
    } else {
        r->l = merge(l, r->l), r->recalc();
        return r;
    }
}
Node* ins(Node* t, Node* n, int pos) {
    auto pa = split(t, pos);
    return merge(merge(pa.first, n), pa.second);
}
// Example application: move the range [l, r) to index k
void mv(Node*& t, int l, int r, int k) {
    Node *a, *b, *c;
    tie(a, b) = split(t, l);
    tie(b, c) = split(b, r - l);
    t = k <= l ? merge(ins(a, b, k), c) : merge(a, ins(c, b, k - r));
}
template <typename T>
struct Point {
   public:
    T x, y;
    Point() : x(0), y(0) {}
    Point(T x_, T y_) : x(x_), y(y_) {}
    template <typename U>
    explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
    Point(const pair<T, T>& p) : x(p.first), y(p.second) {}
    Point(const complex<T>& p) : x(real(p)), y(imag(p)) {}
    explicit operator pair<T, T>() const { return pair<T, T>(x, y); }
    explicit operator complex<T>() const { return complex<T>(x, y); }
    friend ostream& operator<<(ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
    friend istream& operator>>(istream& i, Point& p) { return i >> p.x >> p.y; }
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Point& a, const Point& b) { return !(a == b); }
    Point& operator+=(const Point& p) { x += p.x, y += p.y, return *this; }
    Point& operator-=(const Point& p) { x -= p.x, y -= p.y, return *this; }
    Point& operator*=(const T& t) { x *= t, y *= t, return *this; }
    Point& operator/=(const T& t) { x /= t, y /= t, return *this; }
    friend Point operator+(const Point& a, const Point& b) { return Point(a.x + b.x, a.y + b.y); }
    friend Point operator-(const Point& a, const Point& b) { return Point(a.x - b.x, a.y - b.y); }
    friend Point operator*(const Point& a, const T& t) { return Point(a.x * t, a.y * t); }
    friend Point operator*(const T& t, const Point& a) { return Point(t * a.x, t * a.y); }
    friend Point operator/(const Point& a, const T& t) { return Point(a.x / t, a.y / t); }
    friend T norm(const Point& a) { return a.x * a.x + a.y * a.y; }
    friend auto abs(const Point& p) { return hypot(p.x, p.y); }
    friend T unit(const Point& a) {
        if (a == Point()) return a;
        return a / abs(a);
    }
    friend T int_norm(const Point& a) { return __gcd(a.x, a.y); }
    friend T int_unit(const Point& a) {
        if (a == Point()) return a;
        return a / int_norm(a);
    }
    friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
    friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
    friend T area(const Point& a, const Point& b, const Point& c) { return cross(b - a, c - a); }
    // Returns conj(a) * b
    Point rotation(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
    bool same_dir(const Point& a, const Point& b) { return cross(a, b) == 0 && dot(a, b) > 0; }
    // check if 180 <= s..t < 360
    bool is_reflex(const Point& a, const Point& b) {
        auto c = cross(a, b);
        return c ? (c < 0) : (dot(a, b) < 0);
    }
    // operator < (s,t) for angles in [base,base+2pi)
    bool angle_less(const Point& base, const Point& s, const Point& t) {
        int r = is_reflex(base, s) - is_reflex(base, t);
        return r ? (r < 0) : (0 < cross(s, t));
    }
    auto angle_cmp(const Point& base) {
        return [base](const Point& s, const Point& t) {
            return angle_less(base, s, t);
        };
    }
    auto angle_cmp_center(const Point& center, const Point& dir) {
        return [center, dir](const Point& s, const Point& t) -> bool {
            return angle_less(dir, s - center, t - center);
        };
    }
    // is p in [s,t] taken ccw? 1/0/-1 for in/border/out
    int angle_between(const Point& s, const Point& t, const Point& p) {
        if (same_dir(p, s) || same_dir(p, t)) return 0;
        return angle_less(s, p, t) ? 1 : -1;
    }
};
using pt = Point<ll>;
int orientation(pt a, pt b, pt c) {
    TYPE v = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (v < 0) return -1;  // clockwise
    if (v > 0) return +1;  // counter-clockwise
    return 0;
}
bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool ccw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o > 0 || (include_collinear && o == 0);
}
void replace_with_convex_hull(vector<pt>& a, bool coll = false) {
    if (a.size() == 1) return;
    sort(a.begin(), a.end(), [](pt a, pt b) { return make_pair(a.x, a.y) < make_pair(b.x, b.y); });
    pt p1 = a[0], p2 = a.back();
    vector<pt> up, down;
    up.push_back(p1), down.push_back(p1);
    rep(i, 1, sz(a)) {
        if (i == sz(a) - 1 || cw(p1, a[i], p2, coll)) {
            while (sz(up) >= 2 && !cw(up[sz(up) - 2], up[sz(up) - 1], a[i], coll)) up.pop_back();
            up.push_back(a[i]);
        }
        if (i == sz(a) - 1 || ccw(p1, a[i], p2, coll)) {
            while (sz(down) >= 2 && !ccw(down[sz(down) - 2], down[sz(down) - 1], a[i], coll)) down.pop_back();
            down.push_back(a[i]);
        }
    }
    if (coll && sz(up) == sz(a)) reverse(all(a)), return;
    a = move(up);
    for (int i = sz(down) - 2; i > 0; i--) a.push_back(down[i]);
}
// GAUSSIAN ELIM
const ld eps = 1e-9;
bool IsZero(ld v) {
    return abs(v) < 1e-9;
}
enum GAUSS_MODE { DEGREE, ABS };
template <typename T>
void GaussianElimination(vector<vector<T>>& a, int limit, GAUSS_MODE mode = DEGREE) {
    if (a.empty() || a[0].empty()) return;
    int h = sz(a), w = sz(a[0]);
    for (int i = 0; i < h; i++) assert(w == sz(a[i]));
    assert(limit <= w);
    vector<int> deg(h);
    rep(i, 0, h) rep(j, 0, w) deg[i] += !IsZero(a[i][j]);
    int r = 0;
    rep(c, 0, limit) {
        int id = -1;
        rep(i, r, h) if (!IsZero(a[i][c]) && (id == -1 || (mode == DEGREE && deg[i] < deg[id]) || (mode == ABS && abs(a[id][c]) < abs(a[i][c])))) id = i;
        if (id == -1) continue;
        if (id > r) {
            swap(a[r], a[id]), swap(deg[r], deg[id]);
            rep(j, c, w) a[id][j] = -a[id][j];
        }
        vector<int> nonzero;
        rep(j, c, w) if (!IsZero(a[r][j])) nonzero.push_back(j);
        T inv_a = 1 / a[r][c];
        rep(i, r + 1, h) {
            if (IsZero(a[i][c])) continue;
            T coeff = -a[i][c] * inv_a;
            for (int j : nonzero) {
                if (!IsZero(a[i][j])) deg[i]--;
                a[i][j] += coeff * a[r][j];
                if (!IsZero(a[i][j])) deg[i]++;
            }
        }
        ++r;
    }
    for (r = h - 1; r >= 0; r--) rep(c, 0, limit) if (!IsZero(a[r][c])) {
            T inv_a = 1 / a[r][c];
            for (int i = r - 1; i >= 0; i--) {
                if (IsZero(a[i][c])) continue;
                T coeff = -a[i][c] * inv_a;
                rep(j, c, w) a[i][j] += coeff * a[r][j];
            }
            break;
        }
}
template <typename T>
vector<T> SolveLinearSystem(vector<vector<T>> a, const vector<T>& b, int w) {
    int h = sz(a);
    assert(h == sz(b));
    if (h > 0) assert(w == sz(a[0]));
    for (int i = 0; i < h; i++) a[i].push_back(b[i]);
    // pass ABS if doing on doubles
    GaussianElimination(a, w);
    vector<T> x(w, 0);
    rep(i, 0, h) rep(j, 0, w) if (!IsZero(a[i][j])) {
        x[j] = a[i][w] / a[i][j];
        break;
    }
    return x;
}
// destructive determinant
double det(vector<vector<double>>& a) {
    int n = sz(a);
    double res = 1;
    rep(i, 0, n) {
        int b = i;
        rep(j, i + 1, n) if (fabs(a[j][i]) > fabs(a[b][i])) b = j;
        if (i != b) swap(a[i], a[b]), res *= -1;
        res *= a[i][i];
        if (res == 0) return 0;
        rep(j, i + 1, n) {
            double v = a[j][i] / a[i][i];
            if (v != 0) rep(k, i + 1, n) a[j][k] -= v * a[i][k];
        }
    }
    return res;
}
// 2SAT
struct two_sat_solver {
    int n;
    vector<vector<int>> adj;
    vector<int> value, val, comp, z;
    two_sat_solver(int n = 0) : n(n), adj(n << 1) {}
    int time = 0;
    int add_variable() {
        adj.emplace_back(), adj.emplace_back();
        return n++;
    }
    void either(int u, int v) {
        u = max(2 * u, -1 - 2 * u), v = max(2 * v, -1 - 2 * v);
        adj[u].push_back(v ^ 1), adj[v].push_back(u ^ 1);
    }
    void implies(int u, int v) { either(~u, v); }
    void one_of(int u, int v) { either(u, v), either(~u, ~v); }
    void set_value(int u) { either(u, u); }
    void at_most_one(const vector<int>& arr) {
        if ((int)arr.size() <= 1) return;
        int cur = ~arr[0];
        for (auto u = 2; u < (int)arr.size(); ++u) {
            int next = add_variable();
            either(cur, ~arr[u]), either(cur, next), either(~arr[u], next);
            cur = ~next;
        }
        either(cur, ~arr[1]);
    }
    int dfs(int u) {
        int low = val[u] = ++time, v;
        z.push_back(u);
        for (auto v : adj[u])
            if (!comp[v]) low = min(low, val[v] ?: dfs(v));
        ++time;
        if (low == val[u]) do {
                v = z.back(), z.pop_back(), comp[v] = time;
                if (value[v >> 1] == -1) value[v >> 1] = v & 1;
            } while (v != u);
        return val[u] = low;
    }
    // O(n)
    bool solve() {
        value.assign(n, -1), val.assign(2 * n, 0), comp = val;
        for (auto u = 0; u < n << 1; ++u)
            if (!comp[u]) dfs(u);
        for (auto u = 0; u < n; ++u)
            if (comp[u << 1] == comp[u << 1 ^ 1]) return false;
        return true;
    }
    bool get_value(int u) { return value[u]; }
};
// Kruskal: sort edges, greedily add edges if tree
// Prim: maintain set of partition-crossing edges, pick min weight
template <class G>
auto block_cut_tree(const G& g) {
    int n = (int)g.size();
    vector<int> tin(n, -1), low(n, -1), is_cutpoint(n), stk;
    G components;
    int cutpoints = 0;
    int timer = 0;
    auto dfs = [&](auto self, int u, int p) -> void {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        for (int v : g[u]) {
            if (v == p) continue;
            if (tin[v] == -1) {
                self(self, v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u]) {
                    is_cutpoint[u] = (tin[u] > 0 || tin[v] > 1);
                    typename remove_cref_<decltype(g[0])>::type component{u};
                    while (component.back() != v) component.push_back(stk.back()), stk.pop_back();
                    components.push_back(component);
                }
            } else
                low[u] = min(low[u], tin[v]);
        }
        if (is_cutpoint[u]) cutpoints++;
    };
    for (int u = 0; u < n; ++u)
        if (tin[u] == -1) dfs(dfs, u, -1);
    G forest;
    forest.reserve(cutpoints + components.size());
    vector<int> id(n, -1);
    for (int u = 0; u < n; ++u)
        if (is_cutpoint[u]) id[u] = (int)forest.size(), forest.emplace_back();
    for (auto& comp : components) {
        int node = (int)forest.size();
        forest.emplace_back();
        for (int u : comp)
            if (!is_cutpoint[u]) id[u] = node;
            else
                forest[node].push_back(id[u]), forest[id[u]].push_back(node);
    }
    // id[i] = node in the block-cut forest corresponding to vertex i
    // components[i] = ith component (components also store cutpoints in them)
    // is_cutpoint[i] = 1 if i is a cutpoint, 0 otherwise
    // cutpoints = number of cutpoints (all nodes in block-cut forest numbered <
    // cutpoints are cutpoints)
    // forest = adjacency list for block-cut forest
    // one node for every cutpoint and one node for every component
    return tuple{forest, components, id, is_cutpoint, cutpoints};
}
// BRIDGE TREE
// graph
vector<vector<int>> g(n);  // g[i] contains indices of edges
vector<int> U(m), V(m);    // U[i], V[i] = i^th edge
// adjacent(u, i) when called with u = one member of the i^th edge
// returns the other member
auto adjacent = [&](int u, int i) {
    return (u != U[i]) ? U[i] : V[i];
};
// bridge detection
vector<char> is_bridge(m);
vector<int> level(n), dp(n);
auto dfs_bridge = [&](auto self, int u, int index) {
    // index = index of edge joining u to parent in dfs tree
    // dp[u] = number of back-edges passing over the edge between u and
    // parent
    dp[u] = -1;  // accounting for parent
    for (auto i : g[u]) {
        int v = adjacent(u, i);
        if (level[v] == 0)  // edge to child in dfs tree
            level[v] = level[u] + 1, self(self, v, i), dp[u] += dp[v];
        else if (level[v] < level[u])  // edge upwards
            dp[u]++;
        else if (level[v] > level[u])
            // edge downwards --- needs to be counted because for any back-edge
            // (a, b), a and b are in each other's adjacency list
            dp[u]--;
    }
    if (index != -1 && level[u] > 1 && dp[u] == 0) is_bridge[index] = true;
};
level[0] = 1;
dfs_bridge(dfs_bridge, 0, -1);  // sentinel index
// now all the edges which are bridges are marked
int global_component = 0;
vector<char> visited(n);
vector<vector<int>> tree(1);
// vector<int> component_of(n);
auto dfs_build_bridge_tree = [&](auto self, int u) {
    int current_component = global_component;
    queue<int> q;
    q.push(u);
    visited[u] = 1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        // component_of[v] = current_component;
        for (auto i : g[v]) {
            int w = adjacent(v, i);
            if (!visited[w]) {
                if (is_bridge[i]) {
                    tree.emplace_back(), global_component++;
                    tree[global_component].push_back(current_component);
                    tree[current_component].push_back(global_component);
                    self(self, w);
                } else
                    visited[w] = 1, q.push(w);
            }
        }
    }
};
dfs_build_bridge_tree(dfs_build_bridge_tree, 0);
// CENTROID DECOMPOSITION:
vector<bool> removed(n);
vector<int> sz(n);
auto dfs_sz = [&](const auto& self, int u, int p) -> int {
    sz[u] = 1;
    for (auto v : g[u])
        if (!removed[v] && v != p) sz[u] += self(self, v, u);
    return sz[u];
};
auto dfs_centroid = [&](const auto& self, int u, int p, int bound) -> int {
    for (auto v : g[u])
        if (!removed[v] && v != p && sz[v] >= bound) return self(self, v, u, bound);
    return u;
};
ll ans = 0;
auto dfs_centroid_decomp = [&](const auto& self, int u) -> void {
    int sz_u = dfs_sz(dfs_sz, u, -1);
    int centroid = dfs_centroid(dfs_centroid, u, -1, sz_u / 2);
    removed[centroid] = 1;
    {
        // work here
        // root of the tree here is `centroid`
        // whenever you consider a vertex, check removed[v]
        // example below: compute number of paths of length k passing through u
        basic_string<int> cnt_at_depth{1};
        auto dfs_depth = [&](const auto& Self, int U, int P, int depth) -> void {
            if (depth > k) return;
            if (depth == (int)cnt_at_depth.size()) cnt_at_depth.push_back(1);
            else
                cnt_at_depth[depth]++;
            for (auto V : g[U])
                if (!removed[V] && V != P) Self(Self, V, U, depth + 1);
        };
        auto dfs_acc = [&](const auto& Self, int U, int P, int depth) -> void {
            if (depth > k) return;
            if (k - depth < (int)cnt_at_depth.size()) ans += cnt_at_depth[k - depth];
            for (auto V : g[U])
                if (!removed[V] && V != P) Self(Self, V, U, depth + 1);
        };
        for (auto v : g[centroid])
            if (!removed[v]) dfs_acc(dfs_acc, v, centroid, 1), dfs_depth(dfs_depth, v, centroid, 1);
    }
    for (auto v : g[centroid])
        if (!removed[v]) self(self, v);
};
dfs_centroid_decomp(dfs_centroid_decomp, 0);
// MCMF
const ll INF = numeric_limits<ll>::max() / 4;
typedef vector<ll> VL;
struct MCMF {
    int N;
    vector<vi> ed, red;
    vector<VL> cap, flow, cost;
    vi seen;
    VL dist, pi;
    vector<pii> par;
    // clang-format off
    MCMF(int N)
        : N(N),ed(N),red(N),cap(N, VL(N)),flow(cap),cost(cap),
        seen(N),dist(N),pi(N),par(N) {}
    // clang-format on
    void addEdge(int from, int to, ll cap, ll cost) {
        this->cap[from][to] = cap;
        this->cost[from][to] = cost;
        ed[from].push_back(to);
        red[to].push_back(from);
    }
    void path(int s) {
        fill(all(seen), 0), fill(all(dist), INF);
        dist[s] = 0;
        ll di;
        __gnu_pbds::priority_queue<pair<ll, int>> q;
        vector<decltype(q)::point_iterator> its(N);
        q.push({0, s});
        auto relax = [&](int i, ll cap, ll cost, int dir) {
            ll val = di - pi[i] + cost;
            if (cap && val < dist[i]) {
                dist[i] = val;
                par[i] = {s, dir};
                if (its[i] == q.end()) its[i] = q.push({-dist[i], i});
                else
                    q.modify(its[i], {-dist[i], i});
            }
        };
        while (!q.empty()) {
            s = q.top().second, q.pop(), seen[s] = 1, di = dist[s] + pi[s];
            for (int i : ed[s])
                if (!seen[i]) relax(i, cap[s][i] - flow[s][i], cost[s][i], 1);
            for (int i : red[s])
                if (!seen[i]) relax(i, flow[i][s], -cost[i][s], 0);
        }
        rep(i, 0, N) pi[i] = min(pi[i] + dist[i], INF);
    }
    pair<ll, ll> maxflow(int s, int t) {
        ll totflow = 0, totcost = 0;
        while (path(s), seen[t]) {
            ll fl = INF;
            for (int p, r, x = t; tie(p, r) = par[x], x != s; x = p) fl = min(fl, r ? cap[p][x] - flow[p][x] : flow[x][p]);
            totflow += fl;
            for (int p, r, x = t; tie(p, r) = par[x], x != s; x = p)
                if (r) flow[p][x] += fl;
                else
                    flow[x][p] -= fl;
        }
        rep(i, 0, N) rep(j, 0, N) totcost += cost[i][j] * flow[i][j];
        return {totflow, totcost};
    }
    // If some costs can be negative, call this before maxflow:
    void setpi(int s) {  // (otherwise, leave this out)
        fill(all(pi), INF);
        pi[s] = 0;
        int it = N, ch = 1;
        ll v;
        while (ch-- && it--) rep(i, 0, N) {
                if (pi[i] != INF)
                    for (int to : ed[i])
                        if (cap[i][to])
                            if ((v = pi[i] + cost[i][to]) < pi[to]) pi[to] = v, ch = 1;
            }
        assert(it >= 0);  // negative cost cycle
    }
}

