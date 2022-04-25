// find the longest subsequence s of [0, ..., n - 1] such that
// comparator(s[i], s[i + 1]) is true for all valid i
// no need to use template parameters for strict/non-strict
template <class Compare, bool return_indices = false, bool do_reserve = false>
auto get_lis_indices(int n, Compare&& comparator) {
    std::vector<int> indices;
    if constexpr (do_reserve) indices.reserve(n);
    for (int i = 0; i < n; ++i) {
        int location;
        location = int(
            std::lower_bound(indices.begin(), indices.end(), i, comparator) -
            indices.begin());
        if (location == (int)indices.size())
            indices.push_back(i);
        else
            indices[location] = i;
    }
    if constexpr (return_indices)
        return indices;
    else
        return indices.size();
}

template <class T, bool is_strict = true, bool return_indices = false,
          bool do_reserve = false>
auto get_lis_indices(const std::vector<T>& a) {
    std::vector<std::size_t> indices;
    const auto n = a.size();
    if constexpr (do_reserve) indices.reserve(n);
    const auto comparator = [&a](const std::size_t& x, const std::size_t& y) {
        return a[x] < a[y];
    };
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t location;
        if constexpr (is_strict)
            location = std::lower_bound(std::begin(indices), std::end(indices),
                                        i, comparator) -
                       std::begin(indices);
        else
            location = std::upper_bound(std::begin(indices), std::end(indices),
                                        i, comparator) -
                       std::begin(indices);
        if (location == indices.size())
            indices.push_back(i);
        else
            indices[location] = i;
    }
    if constexpr (return_indices)
        return indices;
    else
        return indices.size();
}

template <class T, bool is_strict = true, bool return_elements = false,
          bool do_reserve = false>
auto get_lis_elements(const std::vector<T>& a) {
    std::vector<T> elements;
    const auto n = a.size();
    if constexpr (do_reserve) elements.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t location;
        if constexpr (is_strict)
            location = std::lower_bound(std::begin(elements),
                                        std::end(elements), a[i]) -
                       std::begin(elements);
        else
            location = std::upper_bound(std::begin(elements),
                                        std::end(elements), a[i]) -
                       std::begin(elements);
        if (location == elements.size())
            elements.push_back(a[i]);
        else
            elements[location] = a[i];
    }
    if constexpr (return_elements)
        return elements;
    else
        return elements.size();
}

