struct SplayTreeByIdx {
    // node information
    struct Data {
        mint x;
        int z;
    };
    // combine_node
    static constexpr Data combine_node(const Data& l, const Data& r) {
        return {l.x + r.x, l.z + r.z};
    }
    // identity node
    static constexpr Data id_node() { return {0, 0}; }
    // update
    struct Update {
        mint a, b;
    };
    // apply_update
    static Data constexpr apply_update(const Update& f, const Data& x) {
        return {x.x * f.a + x.z * f.b, x.z};
    }
    // compose_update: first x then f
    static Update constexpr compose_updates(const Update& f, const Update& x) {
        return {f.a * x.a, f.a * x.b + f.b};
    }
    // identity update
    static Update constexpr id_update() { return {1, 0}; }

    struct Node {
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        Data value = id_node();
        Data aggregate = id_node();
        Update f = id_update();
        bool propagated = true;
        // int z = 0;
        int subtree_size = 0;
        int to_reverse = 0;
    };

    std::deque<Node> buffer;

    Node* NIL = nullptr;
    Node* R;

    void prepareDown(Node* c) {
        if (!c->propagated) {
            if (c->l != NIL) {
                c->l->value = apply_update(c->f, c->l->value);
                c->l->aggregate = apply_update(c->f, c->l->aggregate);
                c->l->f = compose_updates(c->f, c->l->f);
                c->l->propagated = false;
            }
            if (c->r != NIL) {
                c->r->value = apply_update(c->f, c->r->value);
                c->r->aggregate = apply_update(c->f, c->r->aggregate);
                c->r->f = compose_updates(c->f, c->r->f);
                c->r->propagated = false;
            }
            c->f = id_update();
            c->propagated = true;
        }
        if (c->to_reverse) {
            swap(c->l, c->r);
            if (c->l != NIL) c->l->to_reverse ^= 1;
            if (c->r != NIL) c->r->to_reverse ^= 1;
            c->to_reverse = 0;
        }
    }

    void prepareUp(Node* c) {
        c->subtree_size = c->l->subtree_size + c->r->subtree_size + 1;
        c->aggregate =
            combine_node(combine_node(c->l->aggregate, c->value), c->r->aggregate);
    }

    SplayTreeByIdx() {
        if (!NIL) {
            buffer.emplace_back();
            NIL = &buffer.back();
            NIL->l = NIL->r = NIL->p = NIL;
            R = NIL;
        }
    }

    Node*& parentchild(Node* p) {
        if (p->p == NIL) return R;
        if (p->p->l == p)
            return p->p->l;
        else
            return p->p->r;
    }

    void rotL(Node* c) {
        Node* p = c->p;
        parentchild(p) = c;
        c->p = p->p;
        p->p = c;
        if (c->l != NIL) c->l->p = p;
        p->r = c->l;
        c->l = p;
    }

    void rotR(Node* c) {
        Node* p = c->p;
        parentchild(p) = c;
        c->p = p->p;
        p->p = c;
        if (c->r != NIL) c->r->p = p;
        p->l = c->r;
        c->r = p;
    }

    void splay(Node* c) {
        while (c->p != NIL) {
            Node* p = c->p;
            Node* pp = p->p;
            if (p->l == c) {
                if (pp == NIL) {
                    rotR(c);
                } else if (pp->l == p) {
                    rotR(p);
                    rotR(c);
                } else if (pp->r == p) {
                    rotR(c);
                    rotL(c);
                }
            } else {
                if (pp == NIL) {
                    rotL(c);
                } else if (pp->r == p) {
                    rotL(p);
                    rotL(c);
                } else if (pp->l == p) {
                    rotL(c);
                    rotR(c);
                }
            }
            if (pp != NIL) prepareUp(pp);
            if (p != NIL) prepareUp(p);
        }
        prepareUp(c);
    }

    Node* kth_element(int k) {
        if (k >= R->subtree_size) return NIL;
        Node* c = R;
        while (true) {
            prepareDown(c);
            auto cl = c->l;
            if (cl->subtree_size == k) break;
            if (cl->subtree_size > k) {
                c = cl;
                continue;
            }
            k -= cl->subtree_size + 1;
            c = c->r;
        }
        prepareDown(c);
        splay(c);
        return c;
    }

    Node* insert_at(int k, const Data& x) {
        buffer.emplace_back(*NIL);
        Node* nx = &buffer.back();
        // nx->z = nx->subtree_size = 1;
        nx->value = nx->aggregate = x;
        if (k == R->subtree_size) {
            nx->l = R;
            if (R != NIL) R->p = nx;
            R = nx;
            prepareUp(nx);
            return nx;
        }
        auto p = kth_element(k);
        nx->l = p->l;
        nx->r = p;
        R = nx;
        if (p->l != NIL) {
            prepareDown(p->l);
            p->l->p = nx;
        }
        p->p = nx;
        p->l = NIL;
        prepareUp(p);
        prepareUp(nx);
        return nx;
    }

    void erase_at(int k) {
        if (k >= R->subtree_size) return;
        // auto toerase = NIL;
        if (k == 0) {
            kth_element(0);
            prepareDown(R);
            // toerase = R;
            R = R->r;
            if (R != NIL) R->p = NIL;
        } else {
            kth_element(k - 1);
            prepareDown(R);
            auto c = R->r;
            prepareDown(c);
            while (c->l != NIL) {
                c = c->l;
                prepareDown(c);
            }
            auto p = c->p;
            // toerase = c;
            parentchild(c) = c->r;
            if (c->r != NIL) c->r->p = p;
            splay(p);
        }
    }

    Node* between(int l, int r) {
        if (l >= r) return NIL;
        if (l == 0 && r == R->subtree_size) return R;
        if (l == 0) return kth_element(r)->l;
        if (r == R->subtree_size) return kth_element(l - 1)->r;
        auto lp = kth_element(l - 1);
        auto rp = kth_element(r);
        while (rp->l != lp) {
            auto p = lp->p;
            prepareDown(p);
            prepareDown(lp);
            if (p->l == lp)
                rotR(lp);
            else
                rotL(lp);
            prepareUp(p);
            prepareUp(lp);
        }
        return lp->r;
    }

    void reverse(int l, int r) {
        if (l >= r) return;
        auto c = between(l, r);
        c->to_reverse ^= 1;
        prepareDown(c);
        splay(c);
    }

    void update(int l, int r, const Update& f) {
        if (l >= r) return;
        auto c = between(l, r);
        c->value = apply_update(f, c->value);
        c->aggregate = apply_update(f, c->aggregate);
        c->f = compose_updates(f, c->f);
        c->propagated = false;
        prepareDown(c);
        splay(c);
    }

    Data query(int l, int r) {
        if (l >= r) return id_node();
        return between(l, r)->aggregate;
    }
};

