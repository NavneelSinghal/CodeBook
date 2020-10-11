struct trie {
    bool isleaf;
    trie* child[2];
};

trie* create() {
    trie* t = new trie();
    t->isleaf = false;
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
    }
}

void clean(trie* root) {
#ifdef CLEAN
    if (root == NULL) return;
    clean(root->child[0]);
    clean(root->child[1]);
    delete (root);
#endif
}

int maxxor(trie* root, int n) {
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
