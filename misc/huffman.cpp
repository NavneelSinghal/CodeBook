// https://cses.fi/problemset/task/1161/
priority_queue<int, vector<int>, greater<int>> pq;
for (int i = 0; i < n; ++i) pq.emplace(a[i]);
ll ans = 0;
while (pq.size() > 1) {
    auto cost_1 = pq.top();
    pq.pop();
    auto cost_2 = pq.top();
    pq.pop();
    ans += cost_1 + cost_2;
    pq.emplace(cost_1 + cost_2);
}
