#include <cassert>
#include <iostream>

using namespace std;

template <typename T>
class vector {
   private:
    T* a;
    int capacity;
    int length;

   public:
    vector() : capacity(4), length(0), a(new T[4]) {}
    vector(int n) : capacity(n), length(n), a(new T[n]) {}
    vector(int n, T v) : capacity(n), length(n), a(new T[n]) {
        for (int i = 0; i < length; ++i) a[i] = v;
    }
    ~vector() { delete[] a; }
    void push_back(T v) {
        if (length == capacity) {
            T* old = a;
            capacity <<= 1;
            a = new T[capacity];
            for (int i = 0; i < length; ++i) {
                a[i] = old[i];
            }
            delete[] old;
        }
        a[length++] = v;
    }
    void pop_back() { --length; }
    T& back() { return a[length - 1]; }
    int size() { return length; }
    T& operator[](int index) { return a[index]; }
};

// min heap
template <typename T>
class priority_queue {
   private:
    vector<T> a;
    void heapify_down(int i) {
        int left = (i << 1) + 1;
        int right = left + 1;
        int mn = i;
        if (left < a.size() && a[left] < a[i]) mn = left;
        if (right < a.size() && a[right] < a[mn]) mn = right;
        if (mn != i) {
            swap(a[i], a[mn]);
            heapify_down(mn);
        }
    }
    void heapify_up(int i) {
        if (i && a[(i - 1) >> 1] > a[i]) {
            swap(a[i], a[(i - 1) >> 1]);
            heapify_up((i - 1) >> 1);
        }
    }

   public:
    priority_queue() : a() {}
    void push(T val) {
        a.push_back(val);
        int cur = a.size() - 1;
        heapify_up(cur);
    }
    T top() { return a[0]; }
    void pop() {
        a[0] = a.back();
        a.pop_back();
        heapify_down(0);
    }

    int size() { return a.size(); }
};

using ll = long long;

template <typename T, typename V>
class Pair {
   public:
    T F;
    V S;
    Pair() : F(), S() {}
    Pair(T f, V s) : F(f), S(s) {}
    bool operator<(const Pair<T, V> p) {
        if (F == p.F) return S < p.S;
        return F < p.F;
    }
    bool operator>(const Pair<T, V> p) {
        if (F == p.F) return S > p.S;
        return F > p.F;
    }
};

const ll INF = 1e18;
const int MAXN = 5e5 + 5;

int p[MAXN];
ll d[MAXN];
vector<Pair<int, ll>> g[MAXN];
vector<Pair<int, int>> path;

int n, m, s, t, u, v;
ll w;

void dijkstra(int s) {
    for (int i = 0; i < n; ++i) {
        d[i] = INF;
        p[i] = -1;
    }
    d[s] = 0;
    priority_queue<Pair<ll, int>> q;
    q.push(Pair<ll, int>(0ll, s));
    while (q.size()) {
        int v = q.top().S;
        ll dv = q.top().F;
        q.pop();
        if (dv != d[v]) continue;
        auto& gv = g[v];
        int sz = gv.size();
        for (int i = 0; i < sz; ++i) {
            int to = gv[i].F;
            ll len = gv[i].S;
            if (dv + len < d[to]) {
                d[to] = dv + len;
                p[to] = v;
                q.push(Pair<ll, int>(d[to], to));
            }
        }
    }
}

void dijkstra2(int s) {
    for (int i = 0; i < n; ++i) {
        d[i] = INF;
    }
    // (distance, vertex)
    set<pair<long long, int>> s;
    d[s] = 0;
    s.insert({0LL, s});
    while (s.size() > 0) {
        pair<long long, int> p = *s.begin();
        s.erase(s.begin());
        int u = p.second;
        long long len = p.first;
        for (pair<int, long long> edge : g[u]) {
            int v = edge.first;
            long long weight = edge.second;
            if (d[v] > len + weight) {
                s.erase({d[v], v});
                d[v] = len + weight;
                s.insert({d[v], v});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cin >> n >> m >> s >> t;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v >> w;
        g[u].push_back(Pair<int, ll>(v, w));
    }
    dijkstra(s);
    if (d[t] == INF)
        cout << -1 << '\n';
    else {
        cout << d[t];
        while (t != s) {
            path.push_back(Pair<int, int>(p[t], t));
            t = p[t];
        }
        for (int i = 0, j = path.size() - 1; i < j; ++i, --j) {
            auto temp = path[i];
            path[i] = path[j];
            path[j] = temp;
        }
        cout << ' ' << path.size() << '\n';
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i].F << ' ' << path[i].S << '\n';
        }
        cout << '\n';
    }
    return 0;
}
