multiset<int> smaller, larger;
auto cleanup = [&]() {
    if (!smaller.empty()) {
        auto it = prev(smaller.end());
        larger.insert(*it);
        smaller.erase(it);
    }
    while (smaller.size() < larger.size()) {
        auto it = larger.begin();
        smaller.insert(*it);
        larger.erase(it);
    }
};
auto add = [&](int x) {
    smaller.insert(x);
    cleanup();
};
auto remove = [&](int x) {
    auto it = smaller.rbegin();
    if (*it < x)
        larger.erase(larger.lower_bound(x));
    else
        smaller.erase(smaller.lower_bound(x));
    cleanup();
};
auto compute = [&]() -> int {
    auto it = smaller.rbegin();
    if (smaller.size() == larger.size())
        return (*it + *larger.begin()) / 2;
    else
        return *it;
};
