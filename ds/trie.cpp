// frequency of end node stored and not each node
template <char A = 'A', int CHARSET = 26>
struct Trie {
    static const int ROOT = 0;
    int sz;
    vector<vector<int>> trie;
    vector<int> freq;

    Trie(int n) {
        sz = 1;
        trie.assign(CHARSET, vector<int>(n + 2, -1));
        freq.assign(n + 2, 0);
    }

    void insert(string& s) {
        int node = ROOT;
        for (auto c : s) {
            int& next_node = trie[c - A][node];
            if (next_node == -1) {
                next_node = sz++;
            }
            node = next_node;
        }
        freq[node]++;
    }
};

