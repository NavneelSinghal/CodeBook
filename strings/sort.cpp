struct identity {
    template <class T>
    constexpr T operator()(const T& x) const {
        return x;
    }
};

// sort strings of different lengths in time O(total length + max value)
// return permutation as well as the sorted string
template <class T, class U, class F = identity>
auto sort_strings(const std::vector<T>& a, U max_value, F&& mapping = {}) {
    static_assert(
        std::is_same<
            std::remove_cv_t<std::remove_reference_t<decltype(a[0][0])>>, U>{});
    using V = std::basic_string<int>;

    int max_length = 0;
    for (const auto& s : a) max_length = std::max(max_length, (int)s.size());
    int total_length = 0;
    for (const auto& s : a) total_length += (int)s.size();
    int max_value_mapped = mapping(max_value);

    std::vector<V> non_empty(max_length);
    {
        std::vector<std::pair<int, int>> pairs;
        pairs.reserve(total_length);
        for (const auto& s : a)
            for (int i = 0; i < (int)s.size(); ++i)
                pairs.emplace_back(i, mapping(s[i]));
        std::vector<std::pair<int, int>> buf(total_length);
        std::vector<int> cnt(std::max(max_length, max_value_mapped + 1));
        for (auto [l, x] : pairs) ++cnt[x];
        for (int i = 1; i <= max_value_mapped; ++i) cnt[i] += cnt[i - 1];
        for (int i = (int)pairs.size() - 1; i >= 0; --i) {
            const auto& p = pairs[i];
            buf[--cnt[p.second]] = p;
        }
        std::fill(cnt.begin(),
                  cnt.begin() + std::min(max_length, max_value_mapped + 1), 0);
        for (auto [l, x] : buf) ++cnt[l];
        for (int i = 1; i < max_length; ++i) cnt[i] += cnt[i - 1];
        for (int i = (int)buf.size() - 1; i >= 0; --i) {
            const auto& p = buf[i];
            pairs[--cnt[p.first]] = p;
        }
        int count_unique =
            int(std::unique(pairs.begin(), pairs.end()) - pairs.begin());
        pairs.resize(count_unique);
        for (int i = 0; i < count_unique; ++i)
            non_empty[pairs[i].first].push_back(pairs[i].second);
    }

    std::vector<V> strings_of_length(max_length + 1);
    for (int i = 0; i < (int)a.size(); ++i)
        strings_of_length[a[i].size()].push_back(i);

    std::vector<V> q(mapping(max_value) + 1);
    std::queue<int> Q;

    for (int l = max_length; l >= 1; --l) {
        for (auto string_id : strings_of_length[l])
            q[mapping(a[string_id][l - 1])].push_back(string_id);
        while (!Q.empty()) {
            int string_id = Q.front();
            Q.pop();
            q[mapping(a[string_id][l - 1])].push_back(string_id);
        }
        for (auto j : non_empty[l - 1]) {
            for (auto string_id : q[j]) Q.push(string_id);
            q[j].clear();
        }
    }

    std::vector<V> ans(a.size());
    std::vector<int> p(a.size());
    int i = 0;
    for (; i < (int)strings_of_length[0].size(); ++i)
        ans[i] = a[strings_of_length[0][i]], p[i] = strings_of_length[0][i];
    while (!Q.empty()) {
        p[i] = Q.front();
        ans[i] = a[Q.front()];
        ++i;
        Q.pop();
    }
    return std::pair{p, ans};
}

