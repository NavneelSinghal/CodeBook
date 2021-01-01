#include <bits/stdc++.h>
using namespace std;

struct pt {
    double x, y;
}

bool cmp(pt a, pt b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool cw(pt a, pt b, pt c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool ccw(pt a, pt b, pt c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

vector<pt> convex_hull(vector<pt>& a) {
    if (a.size() == 1) return a;
    sort(a.begin(), a.end(), &cmp);
    pt p1 = a[0], p2 = a.back();
    vector<pt> up, down;
    up.push_back(p1);
    down.push_back(p2);
    int n = a.size();
    for (int i = 1; i < n; ++i) {
        if (i == n - 1 || cw(p1, a[i], p2)) {
            while (up.size() >= 2 && !cw(up[up.size() - 2], up.back(), a[i])) {
                up.pop_back();
            }
            up.push_back(a[i]);
        }
        if (i == n - 1 || ccw(p1, a[i], p2)) {
            while (down.size() >= 2 &&
                   !ccw(down[down.size() - 2], down.back(), a[i])) {
                down.pop_back();
            }
            down.push_back(a[i]);
        }
    }
    vector<pt> ans;
    for (auto p : up) {
        ans.push_back(p);
    }
    for (int i = (int)down.size() - 2; i > 0; --i) {
        ans.push_back(down[i]);
    }
    return ans;
}

int main() {
    int n;
    cin >> n;
    vector<pt> a(n);
    for (auto& x : a) cin >> x.x >> x.y;
}
