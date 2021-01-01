template <typename X, typename Y>
X& remin(X& x, const Y& y) {
    return x = (y < x) ? y : x;
}

template <typename X, typename Y>
X& remax(X& x, const Y& y) {
    return x = (x < y) ? y : x;
}

template <typename X, typename Y>
bool ckmin(X& x, const Y& y) {
    return (y < x) ? (x = y, 1) : 0;
}

template <typename X, typename Y>
bool ckmax(X& x, const Y& y) {
    return (x < y) ? (x = y, 1) : 0;
}
