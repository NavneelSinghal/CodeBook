struct Trie {
    static const int CHARSET = 26;
    static const int ROOT = 0;
    int sz;
    vector<vector<int>> trie;
    vector<int> freq;

    // n is the total number of nodes we need to make in the trie
    Trie(int n) {
        sz = 1;
        trie.assign(CHARSET, vector<int>(n + 2, -1));
        freq.assign(n + 2, 0);
    }

    void insert(string& s) {
        int node = ROOT;
        for (auto c : s) {
            int& next_node = trie[c - 'a'][node];
            if (next_node == -1) {
                next_node = sz++;
            }
            freq[next_node]++;
            node = next_node;
        }
    }
};

struct Trie {
    // MAX = number of nodes
    static const int CHARSET = 26;
    static const int ROOT = 0;
    int sz;
    vector<vector<int>> trie;
    vector<int> freq;

    // n is the total number of nodes we need to make in the trie
    Trie(int n) {
        sz = 1;
        trie.assign(n + 2, vector<int>(CHARSET, -1));
        freq.assign(n + 2, 0);
    }

    void insert(string& s) {
        int node = ROOT;
        for (auto c : s) {
            int& next_node = trie[node][c - 'a'];
            if (next_node == -1) {
                next_node = sz++;
            }
            freq[next_node]++;
            node = next_node;
        }
    }
};

/* untested trie for aho corasick */
struct Trie {
    static const int CHARSET = 2;
    static const int ROOT = 0;
    int sz;
    vector<vector<int>> trie;
    vector<int> index, ending, par, fail, go;
    Trie(int n) {
        sz = 1;
        trie.assign(CHARSET, vector<int>(n + 2, -1));
        par.assign(n + 2, -1);
        index.assign(n + 2, -1);
        ending.assign(n + 2, -1);
        fail.assign(n + 2, -1);
        go.assign(n + 2, -1);
    }

    void insert(string& s, int idx) {
        int node = ROOT;
        for (auto c : s) {
            int& next_node = trie[c - 'a'][node];
            if (next_node == -1) {
                next_node = sz++;
                par[next_node] = node;
            }
            node = next_node;
        }
        ending[idx] = node;
        index[node] = idx;
    }

    void build_failure() {
        queue<int> q;
        for (int i = 0; i < CHARSET; ++i) {
            if (trie[0][i] != -1) q.push(trie[0][i]);
        }
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            if (index[x] != -1) {
                go[x] = x;
            } else {
                go[x] = go[fail[x]];
            }
            for (int i = 0; i < CHARSET; ++i) {
                int& next_node = trie[x][i];
                if (next_node != -1) {
                    fail[next_node] = trie[fail[x]][i];
                    q.push(next_node);
                } else {
                    next_node = trie[fail[x]][i];
                }
            }
        }
    }
};


