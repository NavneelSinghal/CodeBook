// suffix array

vector<int32_t> sort_cyclic_shifts(string const& s) {
    int32_t n = s.size();
    const int32_t alphabet = 128;
    vector<int32_t> p(n), c(n), cnt(max(alphabet, n), 0);
    // base case : length = 1, so sort by counting sort
    for (int32_t i = 0; i < n; i++) cnt[s[i]]++;
    for (int32_t i = 1; i < alphabet; i++) cnt[i] += cnt[i - 1];
    for (int32_t i = 0; i < n; i++) p[--cnt[s[i]]] = i;
    c[p[0]] = 0;
    int32_t classes = 1;
    for (int32_t i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i - 1]]) ++classes;
        c[p[i]] = classes - 1;
    }
    // inductive case, sort by radix sort on pairs (in fact you only need to
    // sort by first elements now)
    vector<int32_t> p_new(n), c_new(n);
    for (int32_t h = 0; (1 << h) < n; ++h) {
        for (int32_t i = 0; i < n; i++) {
            p_new[i] = p[i] - (1 << h);
            if (p_new[i] < 0) p_new[i] += n;
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int32_t i = 0; i < n; i++) cnt[c[p_new[i]]]++;
        for (int32_t i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
        for (int32_t i = n - 1; i >= 0; i--) p[--cnt[c[p_new[i]]]] = p_new[i];
        c_new[p[0]] = 0;
        classes = 1;
        for (int32_t i = 1; i < n; i++) {
            pair<int32_t, int32_t> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            pair<int32_t, int32_t> prev = {c[p[i - 1]],
                c[(p[i - 1] + (1 << h)) % n]};
            if (cur != prev) ++classes;
            c_new[p[i]] = classes - 1;
        }
        c.swap(c_new);
    }
    return p;
}
vector<int32_t> suffix_array_construct(string s) {
    s += "$";
    // what about s += " "; ?
    vector<int32_t> sorted_shifts = sort_cyclic_shifts(s);
    // sorted_shifts.erase(sorted_shifts.begin()); - removes the element
    // corresponding to the empty suffix
    return sorted_shifts;
}
