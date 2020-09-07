#include <iostream>
using namespace std;

template <typename T>
class avl {

    private:
        struct node {
            T data;
            node *l, *r;
            int ht;
            node (T v) : data(v), l(NULL), r(NULL), ht(0) {}
        };

        node* root;

        void clear(node *t) {
            if (t == NULL) return;
            clear(t -> l);
            clear(t -> r);
            delete t;
        }

        int height(node *t) {
            if (t == NULL) return -1;
            return t -> ht;
        }

        int balance(node* t) {
            if (t == NULL) return 0;
            return height(t -> l) - height(t -> r);
        }

        node* right_rot (node* &t) {
            if (t -> l != NULL) {
                node* u = t -> l;
                t -> l = u -> r;
                u -> r = t;
                t -> ht = 1 + max(height(t -> l), height(t -> r));
                u -> ht = 1 + max(height(u -> l), t -> ht);
                return u;
            }
            return t;
        }

        node* left_rot (node* &t) {
            if (t -> r != NULL) {
                node* u = t -> r;
                t -> r = u -> l;
                u -> l = t;
                t -> ht = 1 + max(height(t -> l), height(t -> r));
                u -> ht = 1 + max(height(u -> r), t -> ht);
                return u;
            }
            return t;
        }

        node* double_left_rot (node* &t) {
            t -> r = right_rot(t -> r);
            return left_rot(t);
        }

        node* double_right_rot (node* &t) {
            t -> l = left_rot(t -> l);
            return right_rot(t);
        }

        node* begin(node *t) {
            if (t == NULL || (t -> l == NULL)) return t;
            return begin(t -> l);
        }

        node* end(node *t) {
            if (t == NULL || (t -> r == NULL)) return t;
            return end(t -> r);
        }

        node* _insert(node* t, T x) {
            
            if (t == NULL) {
                return new node(x);
            }

            if (x < t -> data) {
                t -> l = _insert(t -> l, x);
                if (height(t -> l) - height(t -> r) == 2) {
                    if (x < t -> l -> data) {
                        t = right_rot(t);
                    } else {
                        t = double_right_rot(t);
                    }
                }
            } else if (x > t -> data) {
                t -> r = _insert(t -> r, x);
                if (height(t -> r) - height(t -> l) == 2) {
                    if (x > t -> r -> data) {
                        t = left_rot(t);
                    } else {
                        t = double_left_rot(t);
                    }
                }
            } else {
                return t;
            }
            
            t -> ht = 1 + max(height(t -> l), height(t -> r));
            return t;
        }

        node* _remove(node* t, T x) {
            
            node* temp;
            
            if (t == NULL) {
                return t;
            } else if (x < t -> data) {
                t -> l = _remove(t -> l, x);
            } else if (x > t -> data) {
                t -> r = _remove(t -> r, x);
            } else if (t -> l && t -> r) {
                temp = begin(t -> r);
                t -> data = temp -> data;
                t -> r = _remove(t -> r, x);
            } else {
                temp = t;
                if (t -> l == NULL) t = t -> r;
                else if (t -> r == NULL) t = t -> l;
                delete temp;
            }

            if (t == NULL) {
                return t;
            }

            t -> ht = 1 + max(height(t -> l), height(t -> r));

            int b = balance(t);
            if (b > 1) {
                if (balance(t -> l) >= 0) {
                    return right_rot(t);
                } else {
                    return double_right_rot(t);
                }
            } else if (b < -1) {
                if (balance(t -> r) <= 0) {
                    return left_rot(t);
                } else {
                    return double_left_rot(t);
                }
            }
            
            return t;
        }

        void print(node* t) {
            if (t == NULL) return;
            print(t -> l);
            std::cout << t -> data << ' ';
            print(t -> r);
        }

    public:
        avl() : root(NULL) {}
        ~avl() { clear(root); }
        void insert(T x) { root = _insert(root, x); }
        void erase(T x) { root = _remove(root, x); }
        void print() { print(root); std::cout << '\n'; }
};


int main() {
    avl<int> t;
    for (int i = 0; i < 100000; ++i) {
        t.insert(rand() % 1000000);
        //t.print();
        t.erase(rand() % 1000000);
    }
    //t.print();
}
