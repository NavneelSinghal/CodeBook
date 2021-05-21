template <class T, class U = T>
bool ckmin(T &a, U &&b) {
    return b < a ? a = std::forward<U>(b), true : false;
}

template <class T, class U = T>
bool ckmax(T &a, U &&b) {
    return a < b ? a = std::forward<U>(b), true : false;
}

template <typename T>
inline T sq(T a) {
    return a * a;
}

template <typename T>
inline T sq_dist(std::pair<T, T> &a, std::pair<T, T> &b) {
    return sq(a.first - b.first) + sq(a.second - b.second);
}

template <typename T>
inline ld dist(std::pair<T, T> &a, std::pair<T, T> &b) {
    return sqrtl(sq_dist(a, b));
}

inline ll isqrt(ll n) {
    ll sq = (ll)sqrtl((ld)n) - 2;
    sq = std::max(sq, ll(0));
    while (sq * sq < n) sq++;
    if ((sq * sq) == n) return sq;
    return sq - 1;
}

inline bool is_sq(ll n) {
    ll w = isqrt(n);
    return w * w == n;
}


