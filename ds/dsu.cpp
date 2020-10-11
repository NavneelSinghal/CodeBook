// dsu implementation
vector<int32_t> par(maxn), siz(maxn);

void make_set(int32_t v) {
    par[v] = v;
    siz[v] = 1;
}

int32_t find_set(int32_t v) {
    if (v == par[v]) return v;
    return par[v] = find_set(par[v]);
}

void union_sets(int32_t a, int32_t b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (siz[a] < siz[b]) swap(a, b);
        par[b] = a;
        siz[a] += siz[b];
    }
}
