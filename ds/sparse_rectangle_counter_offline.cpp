// use multiset in case of duplicates
set<pair<int, int>> points;

int n;
cin >> n;
for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    // add point (x + y, x - y + N) to the set
    // replace with normal points in case chebyshev transformation is not being used
    points.emplace(x + y, x - y + N);
}

vector<vector<int>> Y((2 * N + 1) << 2);

auto build = [&](const auto& self, int i, int l, int r) -> void {
    if (l + 1 == r) {
        // add all points with x = l in sorted order
        auto it = points.lower_bound({l, 0});
        while (it != points.end() && it->first == l)
            Y[i].push_back(it->second), ++it;
    } else {
        self(self, 2 * i, l, (l + r) / 2);
        self(self, 2 * i + 1, (l + r) / 2, r);
        Y[i].resize(Y[2 * i].size() + Y[2 * i + 1].size());
        // merge all points from children
        merge(Y[2 * i].begin(), Y[2 * i].end(), Y[2 * i + 1].begin(),
              Y[2 * i + 1].end(), Y[i].begin());
    }
};

build(build, 1, 0, 2 * N + 1);

// query number of points in [x1, x2) * [y1, y2)
auto query = [&](const auto& self, int x1, int y1, int x2, int y2, int i, int l,
                 int r) -> int {
    if (x2 <= l || r <= x1)  // no overlap
        return 0;
    else if (x1 <= l && r <= x2)  // completely inside
        return int(lower_bound(begin(Y[i]), end(Y[i]), y2) -
                   lower_bound(begin(Y[i]), end(Y[i]), y1));
    else  // query normally
        return self(self, x1, y1, x2, y2, 2 * i, l, (l + r) / 2) +
               self(self, x1, y1, x2, y2, 2 * i + 1, (l + r) / 2, r);
};

