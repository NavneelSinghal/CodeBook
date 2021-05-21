// deletion and duplicates allowed
struct binaryTrie {
    // MAX = number of nodes
    static const int CHARSET = 2;
    static const int MAXLEN = 31;
    static const int ROOT = 0;

    int sz;
    vector<vector<int>> trie;
    vector<int> cnt;
    // vector<int> ends_here;

    // n is the number of total characters, so size needs to be 31 * n
    binaryTrie(int n) {
        sz = 0;
        trie.assign(2, vector<int>(MAXLEN * (n + 2), -1));
        // ends_here.resize(MAXLEN * (n + 2));
        cnt.resize(MAXLEN * (n + 2));
    }

    void insert(int n) {
        int node = ROOT;
        for (int i = MAXLEN - 1; ~i; --i) {
            int& next_node = trie[(n >> i) & 1][node];
            if (next_node == -1) {
                next_node = ++sz;
            }
            node = next_node;
            cnt[node]++;
        }
        // ends_here[node] = n; // or whatever data we need to store at the end
    }

    void remove(int n) {  // assumes that n is already inside the trie - if not,
                          // then just do a simple traversal
        int node = ROOT;
        for (int i = MAXLEN - 1; ~i; --i) {
            int next_node = trie[(n >> i) & 1][node];
            node = next_node;
            cnt[node]--;
        }
    }

    void removeAll(int n) {  // assumes that n is already inside the trie - if
                             // not, then just do a simple traversal
        int node = ROOT;
        for (int i = MAXLEN - 1; ~i; --i) {
            int next_node = trie[(n >> i) & 1][node];
            node = next_node;
            cnt[node] = 0;
        }
    }

    int minXor(int n) {
        int node = ROOT;
        int ans = 0;
        for (int i = MAXLEN - 1; ~i; --i) {
            ans <<= 1;
            int p = (n >> i) & 1;
            if (trie[p][node] == -1 || cnt[trie[p][node]] <= 0) {
                ans ^= 1;
                p ^= 1;
            }
            node = trie[p][node];
        }
        return ans;
    }

    int maxXor(int n) {
        int node = ROOT;
        int ans = 0;
        for (int i = MAXLEN - 1; ~i; --i) {
            ans <<= 1;
            int p = (n >> i) & 1;
            p ^= 1;
            if (trie[p][node] != -1 && cnt[trie[p][node]] > 0) {
                ans ^= 1;
            } else {
                p ^= 1;
            }
            node = trie[p][node];
        }
        return ans;
    }
};

