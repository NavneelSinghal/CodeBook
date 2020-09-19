struct edge {
    int a, b, cost;
};

int n, m, v;
vector<edge> e;
// take INF as longer than any path
const int INF = 1000000000;

void solve() {
    vector<int> d(n, INF);
    d[v] = 0;
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < m; ++j)
            if (d[e[j].a] < INF)
                d[e[j].b] = min(d[e[j].b], d[e[j].a] + e[j].cost);
    // display d, for example, on the screen
}
