// source: Aeren's library
// Each node represents an endpos-equivalent class
// Each class consists of some consecutive suffixes of the longest substring in
// it
// The suffix corresponding to minlen - 1 can be obtained by following the
// suffix link and looking at the longest substring in it
// Adjacency_Type: array<int, X> or map<Char_Type, int> where X is the size of
// the alphabet
template <class Char_Type, class Adjacency_Type>
struct suffix_automaton {
    // Begin States
    // len: length of the longest substring in the class,
    // suffix link,
    // minimum value in the set endpos
    vector<int> len{0}, link{-1}, firstpos{-1};
    vector<bool> is_clone{false};
    vector<Adjacency_Type> next{{}};
    // End States
    int new_state(int l, int sl, int fp, bool c, const Adjacency_Type& adj) {
        len.push_back(l);
        link.push_back(sl);
        firstpos.push_back(fp);
        is_clone.push_back(c);
        next.push_back(adj);
        return (int)len.size() - 1;
    }
    int last = 0;
    void extend(const vector<Char_Type>& s) {
        for (auto c : s) extend(c);
    }
    void extend(Char_Type c) {
        int cur = new_state(len[last] + 1, -1, len[last], false, {}), p = last;
        while (~p && !next[p][c]) next[p][c] = cur, p = link[p];
        if (!~p)
            link[cur] = 0;
        else {
            int q = next[p][c];
            if (len[p] + 1 == len[q])
                link[cur] = q;
            else {
                int clone =
                    new_state(len[p] + 1, link[q], firstpos[q], true, next[q]);
                while (~p && next[p][c] == q) next[p][c] = clone, p = link[p];
                link[q] = link[cur] = clone;
            }
        }
        last = cur;
    }
    int size() const {  // # of states
        return (int)len.size();
    }
    int min_len(int u)
        const {  // length of the shortest string in the endpos equivalent class
        return u ? len[link[u]] + 1 : 0;
    }
    vector<int> terminals() const {
        vector<int> t;
        for (auto u = last; ~u; u = link[u]) t.push_back(u);
        return t;
    }
    vector<int>
        count;  // # of occurrences of substring represented by each states in
                // the string. Calling extend invalidates the array.
    vector<vector<int>> inv_link;
    void precalc_count() {
        int sz = (int)size();
        count.assign(sz, 0), inv_link.assign(sz, {});
        for (auto u = 1; u < sz; ++u) inv_link[link[u]].push_back(u);
        auto dfs = [&](auto self, int u) -> void {
            count[u] = !is_clone[u];
            for (auto v : inv_link[u]) {
                self(self, v);
                count[u] += count[v];
            }
        };
        dfs(dfs, 0);
    }
};



