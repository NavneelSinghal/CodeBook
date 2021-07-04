std::pair<std::vector<int>, std::vector<int>> manacher(const std::string& s) {
    int n = (int)s.size();
    // d_odd[i] = length of palindrome centered at i
    // d_even[i] = length of palindrome centered at space before i
    std::vector<int> d_odd(n), d_even(n);
    // computing d_odd
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int len = (i > r) ? 1 : std::min(d_odd[l + r - i], r - i + 1);
        while (len <= i && i + len < n && s[i - len] == s[i + len]) ++len;
        d_odd[i] = len;
        --len;
        if (i + len > r) {
            l = i - len;
            r = i + len;
        }
    }
    // computing d_even
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int len = (i > r) ? 0 : std::min(d_even[l + r - i + 1], r - i + 1);
        while (len < i && i + len < n && s[i - len - 1] == s[i + len]) ++len;
        d_even[i] = len;
        --len;
        if (i + len > r) {
            l = i - len - 1;
            r = i + len;
        }
    }
    return {d_odd, d_even};
}
