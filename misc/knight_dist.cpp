using ll = long long;
using pt = std::pair<ll, ll>;
ll getDist(const pt& a, const pt& b) {
    ll dx = abs(a.x - b.x);
    ll dy = abs(a.y - b.y);
    if (dx == 1 && dy == 0) return 3;
    if (dx == 0 && dy == 1) return 3;
    if (dx == 2 && dy == 2) return 4;
    ll lb = (dx + 1) / 2;
    lb = std::max(lb, (dy + 1) / 2);
    lb = std::max(lb, (dx + dy + 2) / 3);
    while ((lb & 1) != ((dx + dy) & 1)) lb++;
    return lb;
}
