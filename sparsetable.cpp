// sparse table
const int N = 1e6 + 6;
const int Log = 26;

int sparse_table[Log][N];

void build_sparse_table(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        sparse_table[0][i] = a[i];
    }
    for (int j = 0; j < Log - 1; ++j) {
        for (int i = 0; i + (2 << j) <= n; ++i) {
            sparse_table[j + 1][i] =
                min(sparse_table[j][i], sparse_table[j][i + (1 << j)]);
        }
    }
}

//[l, r)
int query(int l, int r) {
    int sz = __lg(r - l);
    return min(sparse_table[sz][l], sparse_table[sz][r - (1 << sz)]);
}
