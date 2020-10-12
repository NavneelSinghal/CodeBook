// fast doubling using
// f(2k) = f(k) (2f(k+1) - f(k))
// f(2k+1) = f(k+1)^2 + f(k)^2

pair<int, int> fib (int n) {
    if (n == 0)
        return {0, 1};
    auto p = fib(n >> 1);
    int c = p.first * (2 * p.second - p.first);
    int d = p.first * p.first + p.second * p.second;
    if (n & 1)
        return {d, c + d};
    else
        return {c, d};
}

// fibonacci encoding

// suppose f contains the fibonacci numbers
// this returns the indices of fibonacci numbers used in the fibonacci encoding of a
// make sure f's last element is > a
vector<int> fibonacci_encoding (int a, vector<int>& f) {
    int i = 0;
    assert(f.back() > a);
    while (i + 1 < (int)f.size() && f[i + 1] <= a) ++i;
    vector<int> ans;
    while (i >= 0) {
        if (f[i] <= a) {
            ans.push_back(i);
            a -= f[i];
            i -= 2;
        } else {
            --i;
        }
    }
    assert(a == 0);
    return ans;
}
