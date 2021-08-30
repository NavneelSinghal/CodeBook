// for maintaining subtree information, look at
// https://codeforces.com/blog/entry/67637

template <class node>
struct link_cut_tree {
    bool connected(node* u, node* v) { return lca(u, v) != NULL; }
    int depth(node* u) {
        // make root(LCT component)->u a preferred path, then root and u are in
        // the same LCT node, and since this node has root in it, this is the
        // root node of the LCT component. note that after accessing u, u is the
        // root of this splay tree, and has no left child, so size of subtree of
        // left child = length of path from u to the root
        access(u);
        return get_sz(u->ch[0]);
    }
    // get root of LCT component
    node* get_root(node* u) {
        // make root(LCT component)->u a preferred path, then since our splay
        // tree is sorted by key as depths, root of this LCT component (not
        // node) will be leftmost thing in the current splay tree (of which u is
        // a root)
        access(u);
        while (u->ch[0]) u = u->ch[0], u->push();
        return access(u), u;
    }
    // get k-th parent on path to root
    node* ancestor(node* u, int k) {
        // replace k by the depth of the answer
        k = depth(u) - k;
        assert(k >= 0);
        while (true) {
            int sz = get_sz(u->ch[0]);
            // if there are exactly k ancestors, access u and return u
            if (sz == k) return access(u), u;
            // jump downwards by some amount if less than k, else go left
            if (sz < k)
                k -= sz + 1, u = u->ch[1];
            else
                u = u->ch[0];
            u->push();
        }
        assert(0);
    }
    node* lca(node* u, node* v) {
        if (u == v) return u;
        access(u);
        access(v);
        // if u's path parent is null, then it is not a root anymore
        if (!u->p) return NULL;
        // bring u to the top of the splay tree component
        u->splay();
        // since we marked accessed u and then v, the lca should either be u or
        // the path parent of u, since u is now the root of the splay tree
        // component which branches off at the edge joining u and the
        // path-parent of u
        return u->p ? u->p : u;
    }
    // make u parent of v
    void link(node* u, node* v) {
        // remove this assert to get better performance if you're getting tle
        // and are sure that they aren't connected already
        assert(!connected(u, v));
        // make v the root of the represented component of the forest that v is
        // a part of
        make_root(v);
        // access u and make it the root of the component of the splay tree it
        // is in
        access(u);
        // to make u the parent of v in the represented tree, we need to add it
        // to the left subtree of v in the splay tree component, and since u
        // doesn't have a right child in the splay tree, it suffices to set u as
        // the left child of v in the splay tree
        set_link(v, u, 0);
        v->update();
    }
    // cut u from its parent
    void cut(node* u) {
        // make u the root of the splay component
        access(u);
        // remove the left child, by similar reasoning as above, and set path
        // parent to none
        u->ch[0]->p = NULL;
        u->ch[0] = NULL;
        u->update();
    }
    // if u, v adj in tree
    void cut(node* u, node* v) { cut(depth(u) > depth(v) ? u : v); }
    // make u root of LCT component
    void make_root(node* u) {
        // mark u to root as preferred path and make u the root of its splay
        // tree component
        access(u);
        // reverse so that u is now in fact the leftmost vertex in the order -
        // this corresponds to reversing the whole path from u to the root
        u->reverse();
        // since we have just swapped the children, we will need to maintain the
        // invariant that is maintained after access (root should be leftmost
        // and not have any children - i.e., it should be a leaf), and the path
        // pointers should be fixed
        access(u);
        assert(!u->ch[0] && !u->ch[1]);
    }
    // puts u on the preferred path, splay (right subtree is empty after this)
    void access(node* u) {
        for (node *v = u, *pre = NULL; v; v = v->p) {
            v->splay();  // now update virtual children
            if (pre) v->update_vsub(pre, false);
            if (v->ch[1]) v->update_vsub(v->ch[1], true);
            v->ch[1] = pre;
            v->update();
            pre = v;
        }
        u->splay();
        assert(!u->ch[1]);
    }

    node* operator[](int i) { return &data[i]; }
    int operator[](node* i) { return i - &data[0]; }

    std::vector<node> data;
    link_cut_tree(int n) : data(n) {}
};

template <class pnode>
struct splay_tree {
    std::array<pnode, 2> ch;
    pnode p;
    bool rev;
    int sz;
    splay_tree() {
        ch[0] = ch[1] = p = NULL;
        rev = 0;
        sz = 1;
    }
    friend int get_sz(pnode u) { return u ? u->sz : 0; }
    virtual void update() { sz = 1 + get_sz(ch[0]) + get_sz(ch[1]); }
    virtual void push() {
        if (rev) {
            if (ch[0]) ch[0]->reverse();
            if (ch[1]) ch[1]->reverse();
            rev = 0;
        }
    }
    virtual void reverse() {
        rev ^= 1;
        std::swap(ch[0], ch[1]);
    }
    int dir() {
        if (!p) return -2;               // root of LCT component
        if (p->ch[0] == this) return 0;  // left child
        if (p->ch[1] == this) return 1;  // right child
        return -1;                       // root of current splay tree
    }
    bool is_root() { return dir() < 0; }
    friend void set_link(pnode u, pnode v, int d) {
        if (v) v->p = u;
        if (d >= 0) u->ch[d] = v;
    }
    // assume p and p->p propagated
    void rotate() {
        assert(!is_root());
        int x = dir();
        pnode g = p;
        set_link(g->p, static_cast<pnode>(this), g->dir());
        set_link(g, ch[x ^ 1], x);
        set_link(static_cast<pnode>(this), g, x ^ 1);
        g->update();  // update();
    }
    // bring this to top of splay tree
    void splay() {
        while (!is_root() && !p->is_root()) {
            p->p->push(), p->push(), push();
            dir() == p->dir() ? p->rotate() : rotate();
            rotate();
        }
        if (!is_root()) p->push(), push(), rotate();
        push();
        update();
    }
};

// lazy should be I'm ok my children are not ok (only affect children)
// calling u->update requires that there is no lazy in u
// use reverse when the order of the children matters (fix the node here)
struct node : splay_tree<node*> {
    using ll = std::int64_t;
    ll x, sub, vsub;
    node() : splay_tree() { sub = vsub = 0; }
    void update() override {
        splay_tree::update();
        sub = x + vsub;
        sub += (ch[0] ? ch[0]->sub : 0);
        sub += (ch[1] ? ch[1]->sub : 0);
    }
    void update_vsub(node* v, bool add) { vsub += (add ? +1 : -1) * v->sub; }
    void push() override { splay_tree::push(); }
    void reverse() override { splay_tree::reverse(); }
};

struct node_sub_sum : splay_tree<node_sub_sum*> {
    using ll = std::int64_t;
    int rsz, vrsz;
    ll x, sub, vsub, addsub, addvsub;
    node_sub_sum() : splay_tree() {
        x = vrsz = sub = vsub = addsub = addvsub = 0;
        rsz = 1;
    }
    void update() override {
        splay_tree::update();
        assert(addsub == 0);
        sub = x + vsub;
        rsz = 1 + vrsz;
        for (int i = 0; i < 2; ++i)
            if (ch[i]) sub += ch[i]->sub, rsz += ch[i]->rsz;
    }
    void update_vsub(node_sub_sum* v, bool add) {
        if (!add) v->apply(addvsub);
        vsub += (add ? +1 : -1) * v->sub;
        vrsz += (add ? +1 : -1) * v->rsz;
        if (add) v->apply(-addvsub);
    }
    void push() override {
        splay_tree::push();
        if (addsub) {
            if (ch[0]) ch[0]->apply(addsub);
            if (ch[1]) ch[1]->apply(addsub);
            addsub = 0;
        }
    }
    void reverse() override { splay_tree::reverse(); }
    void apply(ll i) {
        x += i;
        sub += (ll)i * rsz;
        vsub += (ll)i * vrsz;
        addsub += i;
        addvsub += i;
    }
};


