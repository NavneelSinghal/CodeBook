// frequency of end node stored and not each node
template <bool multi = true, char A = 'a', int CHARSET = 5>
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

    void insert(const string& s) {
        int node = ROOT;
        for (auto& c : s) {
            int& next_node = trie[c - A][node];
            if (next_node == -1) {
                next_node = sz++;
            }
            node = next_node;
        }
        if constexpr (multi) {
            freq[node]++;
        } else {
            freq[node] = 1;
        }
    }

    int getfreq(const string& s) {
        int node = ROOT;
        for (auto& c : s) {
            int& next_node = trie[c - A][node];
            if (next_node == -1) {
                return 0;
            }
            node = next_node;
        }
        return freq[node];
    }

    vector<string> listall() {
        vector<string> ans;
        const auto rec = [&](const auto& self, int node, string& s) -> void {
            if (freq[node]) {
                for (int i = 0; i < freq[node]; ++i) ans.push_back(s);
            }
            for (int i = 0; i < CHARSET; ++i) {
                if (trie[i][node] != -1) {
                    s += A + i;
                    self(self, trie[i][node], s);
                    s.pop_back();
                }
            }
        };
        string s = "";
        rec(rec, ROOT, s);
        return ans;
    }
};

