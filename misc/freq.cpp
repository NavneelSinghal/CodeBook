template <class T>
vector<int> frequency(vector<T> a) {
    if (a.size() == 0) return vector<int>();
    sort(begin(a), end(a));
    vector<int> ans{0};
    int n = a.size();
    for (int i = 0; i < n; ++i)
        if (i == 0 || a[i] == a[i - 1])
            ans.back()++;
        else
            ans.push_back(1);
    return ans;
}
