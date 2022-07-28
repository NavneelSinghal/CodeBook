// find the longest subsequence s of [0, ..., n - 1] such that
// comparator(s[i], s[i + 1]) is true for all valid i
// no need to use template parameters for strict/non-strict
template <bool return_indices = false, bool do_reserve = false>
auto get_lis_indices(int n, auto&& comparator) {
    std::vector<int> indices, position;
    if constexpr (return_indices) position = std::vector<int>(n);
    if constexpr (do_reserve) indices.reserve(n);
    for (int i = 0; i < n; ++i) {
        auto location = static_cast<int>(
            std::lower_bound(indices.begin(), indices.end(), i, comparator) -
            indices.begin());
        if constexpr (return_indices) position[i] = location;
        if (location == static_cast<int>(indices.size()))
            indices.push_back(i);
        else
            indices[location] = i;
    }
    if constexpr (return_indices) {
        auto remaining = static_cast<int>(indices.size());
        std::vector<int> ans(remaining);
        for (int i = n - 1; i >= 0; --i)
            if (position[i] == remaining - 1) ans[--remaining] = i;
        return ans;
    } else
        return indices.size();
}

template <bool return_values = false, bool do_reserve = false>
auto get_lis(auto Begin, auto End, auto&& comparator) {
    std::vector<std::decay_t<decltype(*Begin)>> ans;
    int n = static_cast<int>(End - Begin);
    if constexpr (do_reserve) ans.reserve(n);
    for (int i = 0; Begin != End; ++Begin, ++i) {
        auto&& element = *Begin;
        auto location = static_cast<int>(
            std::lower_bound(ans.begin(), ans.end(), element, comparator) -
            ans.begin());
        if (location == static_cast<int>(ans.size()))
            ans.push_back(element);
        else
            ans[location] = element;
    }
    return ans;
}
