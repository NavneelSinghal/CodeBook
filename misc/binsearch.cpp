template <typename I, typename P, bool b>
I bin_search_split(const P& predicate, I l, I r) {
    --l, ++r;
    while (r - l > 1) {
        // auto mid = std::midpoint(l, r);
        auto mid = l + (r - l) / 2;
        if (predicate(mid))
            r = mid;
        else
            l = mid;
    }
    if constexpr (b) {
        return r;
    } else {
        return l;
    }
}

// returns first i in [l, r] with p(i) true, and if none found, returns r + 1
template <typename I, typename P>
I first_true(const P& p, I l, I r) {
    return bin_search_split<I, P, true>(p, l, r);
}

// returns last i in [l, r] with p(i) false, and if none found, returns l - 1
template <typename I, typename P>
I last_false(const P& p, I l, I r) {
    return bin_search_split<I, P, false>(p, l, r);
}
