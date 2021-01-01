// frequency of end node stored and not each node
struct Trie {
    static const int CHARSET = 26;
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
            int& next_node = trie[c - 'A'][node];
            if (next_node == -1) {
                next_node = sz++;
            }
            node = next_node;
        }
        freq[node]++;
    }
};



// might be better than below due to caching - change in the binary trie too maybe

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
