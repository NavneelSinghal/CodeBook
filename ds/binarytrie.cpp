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

// if we need to make a binary trie on an array

struct binaryTrie {
    // MAX = number of nodes
    static const int CHARSET = 2;
    static const int MAXLEN = 31;
    static const int ROOT = 0;

    int sz;
    vector<vector<int>> trie;
    vector<int> cnt;
    vector<int> ends_here;
    vector<int> a;

    // n is the number of total characters, so size needs to be 31 * n
    binaryTrie(int n) {
        sz = 0;
        trie.assign(2, vector<int>(MAXLEN * (n + 2), -1));
        a.resize(n + 1);
        ends_here.resize(MAXLEN * (n + 2));
        cnt.resize(MAXLEN * (n + 2));
    }

    void insert(int n) {
        int node = ROOT;
        int to_insert = a[n];
        for (int i = MAXLEN - 1; ~i; --i) {
            int& next_node = trie[(to_insert >> i) & 1][node];
            if (next_node == -1) {
                next_node = ++sz;
            }
            node = next_node;
            cnt[node]++;
        }
        ends_here[node] = n;
    }

    void remove(int n) {  // assumes that n is already inside the trie - if not,
                          // then just do a simple traversal
        int node = ROOT;
        int to_remove = a[n];
        for (int i = MAXLEN - 1; ~i; --i) {
            node = trie[(to_remove >> i) & 1][node];
            cnt[node]--;
        }
    }

    pair<int, int> minxor(int n) {
        int node = ROOT;
        int ans = 0;
        int to_use = a[n];
        for (int i = MAXLEN - 1; ~i; --i) {
            ans <<= 1;
            int p = (to_use >> i) & 1;
            int next_node = trie[p][node];
            if (next_node == -1 || cnt[next_node] <= 0) {
                ans ^= 1;
                p ^= 1;
            }
            node = trie[p][node];
        }
        return make_pair(ans, ends_here[node]);
    }
};

// when assuming all distinct, without deletion
struct binaryTrie {
    // MAX = number of nodes
    static const int CHARSET = 2;
    static const int MAXLEN = 31;
    static const int ROOT = 0;
    int sz;
    vector<vector<int>> trie;

    // n is the number of total characters, so size needs to be 31 * n
    binaryTrie(int n) {
        sz = 1;
        trie.assign(2, vector<int>(MAXLEN * (n + 2), -1));
    }

    void insert(int n) {
        int node = ROOT;
        for (int i = MAXLEN - 1; ~i; --i) {
            int& next_node = trie[(n >> i) & 1][node];
            if (next_node == -1) {
                next_node = sz++;
            }
            node = next_node;
        }
    }

    int maxXor(int n) {
        int node = ROOT;
        int ans = 0;
        for (int i = MAXLEN - 1; ~i; --i) {
            ans <<= 1;
            int p = (n >> i) & 1;
            if (trie[p ^ 1][node] != -1) {
                p ^= 1;
                ans ^= 1;
            }
            node = trie[p][node];
        }
        return ans;
    }
};

struct binaryTrie {
    struct trie {
        // bool isLeaf;
        // can also implement using a counter for frequency
        // if implementing delete, search etc, add an isLeaf flag
        trie* child[2];
    };

    trie* node;

    trie* create() {
        trie* t = new trie();
        memset(t->child, 0, sizeof t->child);
        return t;
    }

    void add(trie* root, int n) {
        int p = 0;
        for (int i = 31; ~i; --i) {
            p = (n >> i) & 1;
            if (root->child[p] == NULL) {
                root->child[p] = create();
            }
            root = root->child[p];
            root->freq++;
        }
    }

    void clean(trie* root) {
        if (root == NULL) return;
        clean(root->child[0]);
        clean(root->child[1]);
        delete (root);
    }

    // max xor with integer n
    int maxXor(trie* root, int n) {
        int ans = 0;
        for (int i = 31; ~i; --i) {
            int p = (n >> i) & 1;
            if (root->child[p ^ 1] != NULL) {
                p ^= 1;
            }
            root = root->child[p];
            ans <<= 1;
            ans |= p ^ ((n >> i) & 1);
        }
        return ans;
    }

    binaryTrie() { root = create(); }
    ~binaryTrie() { clean(root); }

    void insert(int n) { add(root, n); }
    // TODO: add functionality to delete and search

    void maxXor(int n) { maxXor(root, n); }
};
