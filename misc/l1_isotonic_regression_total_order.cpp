{
    // https://drops.dagstuhl.de/opus/volltexte/2018/10027/pdf/OASIcs-SOSA-2019-1.pdf
    auto element = [&](int i) -> ll {
        return a[i];
    };
    auto weight = [](int) -> ll {
        return 1;
    };

    auto weighted_isotonic_regression_l1_total_order =
        [](int n, const auto& element, const auto& weight) -> vector<ll> {
        struct Pair {
            ll pos, val;
            Pair(int pos_ = 0, int val_ = 0) : pos{pos_}, val{val_} {}
            bool operator<(const Pair& other) const { return pos < other.pos; }
        };
        vector<ll> p(n);
        deque<Pair> pq;  // max heap
        for (int i = 0; i < n; ++i) {
            pq.emplace_back(element(i), 2 * weight(i));
            push_heap(begin(pq), end(pq));
            ll s = -weight(i);
            auto* b = &pq[0];
            while (s + b->val <= 0) {
                s += b->val;
                pop_heap(begin(pq), end(pq));
                pq.pop_back();
                b = &pq[0];
            }
            b->val += s;
            p[i] = b->pos;
        }
        for (int i = n - 2; i >= 0; --i) p[i] = min(p[i], p[i + 1]);
        return p;
    };
}
