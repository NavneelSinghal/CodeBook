// 0-based indexing for API
// 1-based indexing for internals
template <typename T>
struct Fenwick {
    using ll = long long;
    ll n;
    pbds::unordered_map<ll, T, hashing::custom_hash<ll>> t;
    Fenwick(ll n) : n(n) {}
    // reserve roughly q * log q
    void reserve(int sz) {
        assert(sz > 0 && !(sz & (sz - 1)));
        t.resize(sz);
    }
    // prefix sum [0, i)
    T query(ll i) {
        T s = 0;
        while (i) {
            auto it = t.find(i);
            if (it != t.end()) s += it->second;
            i -= i & (-i);
        }
        return s;
    }
    // range query [l, r)
    T query(ll l, ll r) { return query(r) - query(l); }
    // increase a[i] by v
    void update(ll i, T v) {
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
    ll max_right(F&& f) {
        T sum = 0;
        ll pos = 0;
        for (ll i = __lg(n); i >= 0; --i) {
            ll x = pos | (1LL << i);
            if (x <= n) {
                auto it = t.find(x);
                T s = sum + (it == t.end() ? 0 : it->second);
                if (f(s, x)) {
                    pos = x;
                    sum = s;
                }
            }
        }
        return pos;
    }
};


