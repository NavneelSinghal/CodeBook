template <typename I, typename P, bool b>
I bin_search_split(I l, I r, const P &predicate) {
    --l, ++r;
    while (r - l > 1) {
        auto mid = l + (r - l) / 2;
        if (predicate(mid))
            l = mid;
        else
            r = mid;
    }
    if constexpr (b) {
        return r;
    } else {
        return l;
    }
}

// returns first i in [l, r], p(i) false, and if none found, returns r + 1
template <typename I, typename P>
I first_false(I l, I r, const P &p) {
    return bin_search_split<I, P, true>(l, r, p);
}

// returns last i in [l, r], p(i) true, and if none found, returns l - 1
template <typename I, typename P>
I last_true(I l, I r, const P &p) {
    return bin_search_split<I, P, false>(l, r, p);
}

