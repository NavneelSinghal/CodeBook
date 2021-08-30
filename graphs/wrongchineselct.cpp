#ifndef LOCAL
    #pragma GCC optimize("O3,unroll-loops")
    #pragma GCC target("avx,avx2,sse,sse2,sse3,sse4,popcnt,bmi,bmi2,lzcnt")
#endif

#include "bits/stdc++.h"

#ifdef DEBUG
    #include "includes/debug/debug.hpp"
#else
    #define debug(...) 0
#endif

// 1-indexed
struct LCT {
    struct Node {
        std::array<int, 2> child{};
        int parent{0};
        // int virtual_info, size;
        bool rev{false};
    };
    std::vector<Node> nodes{};
    std::vector<int> stack{};
    int top{0};

    LCT(int n) : nodes(n + 1), stack(n + 1) {}

    bool not_root(int u) {
        auto [x, y] = nodes[nodes[u].parent].child;
        return u == x || u == y;
    }

    void rotate(int x) {
        int y = nodes[x].parent;
        int z = nodes[y].parent;
        int k = x == nodes[y].child[1];
        if (not_root(y)) nodes[z].child[y == nodes[z].child[1]] = x;
        nodes[x].parent = z;
        nodes[y].child[k] = nodes[x].child[k ^ 1];
        nodes[nodes[x].child[k ^ 1]].parent = y;
        nodes[x].child[k ^ 1] = y;
        nodes[y].parent = x;
        pushup(y);
        pushup(x);
    }

    void splay(int x) {
        int u = x;
        stack[++top] = x;
        while (not_root(u)) stack[++top] = u = nodes[u].parent;
        while (top) pushdown(stack[top--]);
        while (not_root(x)) {
            int y = nodes[x].parent;
            int z = nodes[y].parent;
            if (not_root(y))
                (x == nodes[y].child[1]) ^ (y == nodes[z].child[1]) ? rotate(x)
                                                                    : rotate(y);
            rotate(x);
        }
    }

    void access(int x) {
        for (int y = 0; x; x = nodes[y = x].parent) {
            splay(x);
            // nodes[x].virtual_info += nodes[nodes[x].child[1]].size;
            nodes[x].child[1] = y;
            // nodes[x].virtual_info -= nodes[nodes[x].child[1]].size;
            pushup(x);
        }
    }

    void make_root(int x) {
        access(x);
        splay(x);
        reverse(x);
    }

    void split(int x, int y) {
        make_root(x);
        access(y);
        splay(y);
    }

    void link(int x, int y) {
        make_root(x);
        access(y);
        splay(y);
        nodes[x].parent = y;
        // nodes[y].virtual_info += nodes[x].size;
    }

    void reverse([[maybe_unused]] int x) {
        std::swap(nodes[x].child[0], nodes[x].child[1]);
        nodes[x].rev = !nodes[x].rev;
    }

    void pushup([[maybe_unused]] int x) {
        // auto [u, v] = nodes[x].child;
        // nodes[x].size
        //    = nodes[u].size + nodes[v].size + nodes[x].virtual_info + 1;
    }

    void pushdown([[maybe_unused]] int x) {
        if (nodes[x].rev) {
            auto [u, v] = nodes[x].child;
            reverse(u);
            reverse(v);
            nodes[x].rev = false;
        }
    }

    int get_root(int x) {
        access(x);
        while (nodes[x].child[0]) x = nodes[x].child[0];
        access(x);
        return x;
    }
};

using namespace std;

using ll = int64_t;
using ld = long double;

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // cout << setprecision(20) << fixed;
    int _tests = 1;
    // cin >> _tests;
    for (int _test = 1; _test <= _tests; ++_test) {
        // cout << "Case #" << _test << ": ";
        int n, m;
        cin >> n >> m;
        LCT lct(n);
        while (m--) {
            string type;
            cin >> type;
            int u, v;
            cin >> u >> v;
            if (type[0] == 'c') {
                lct.make_root(u);
                if (lct.get_root(v) == u) {
                    cout << "YES\n";
                } else {
                    cout << "NO\n";
                }
            } else if (type[0] == 'a') {
                lct.link(u, v);
            } else {
                lct.split(u, v);
            }
        }
    }
}
