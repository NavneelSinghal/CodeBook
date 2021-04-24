#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#define rep(i, n) for (int(i) = 0; (i) < (int)(n); ++(i))
#define rer(i, l, u) for (int(i) = (int)(l); (i) <= (int)(u); ++(i))
#define reu(i, l, u) for (int(i) = (int)(l); (i) < (int)(u); ++(i))
#if defined(_MSC_VER) || __cplusplus > 199711L
    #define aut(r, v) auto r = (v)
#else
    #define aut(r, v) typeof(v) r = (v)
#endif
#define each(it, o) for (aut(it, (o).begin()); it != (o).end(); ++it)
#define all(o) (o).begin(), (o).end()
#define pb(x) push_back(x)
#define mp(x, y) make_pair((x), (y))
#define mset(m, v) memset(m, v, sizeof(m))
#define INF 0x3f3f3f3f
#define INFL 0x3f3f3f3f3f3f3f3fLL
using namespace std;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<pair<int, int> > vpii;
typedef long long ll;
typedef vector<long long> vl;
typedef pair<long long, long long> pll;
typedef vector<pair<long long, long long> > vpll;
typedef vector<string> vs;
typedef long double ld;
template <typename T, typename U>
inline void amin(T &x, U y) {
    if (y < x)
        x = y;
}
template <typename T, typename U>
inline void amax(T &x, U y) {
    if (x < y)
        x = y;
}

struct Xor128 {
    unsigned x, y, z, w;
    Xor128() : x(123456789), y(362436069), z(521288629), w(88675123) {}
    unsigned next() {
        unsigned t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
    }
    //手抜き
    inline unsigned next(unsigned n) { return next() % n; }
};

// bottom upなTreap
//脱再帰！
// randomized binary searchにするにはchoiceRandomlyを
//	bool choiceRandomly(Ref l, Ref r) { return rng.next(l->size + r->size) <
//l->size; } に書き換えるだけでよい。
template <typename Node>
struct BottomupTreap {
    Xor128 rng;
    typedef Node *Ref;
    static int size(Ref t) { return !t ? 0 : t->size; }

    unsigned nextRand() { return rng.next(); }

   private:
    bool choiceRandomly(Ref l, Ref r) { return l->priority < r->priority; }

   public:
    Ref join(Ref l, Ref r) {
        if (!l)
            return r;
        if (!r)
            return l;

        Ref t = NULL;
        unsigned long long dirs = 0;
        int h;
        for (h = 0;; ++h) {
            if (h >= sizeof(dirs) * 8 - 2) {
                // dirsのオーバーフローを防ぐために再帰する。
                //あくまでセーフティガードなのでバランスは多少崩れるかもしれない
                t = join(l->right, r->left);
                dirs = dirs << 2 | 1;
                h++;
                break;
            }
            dirs <<= 1;
            if (choiceRandomly(l, r)) {
                Ref c = l->right;
                if (!c) {
                    t = r;
                    r = r->parent;
                    break;
                }
                l = c;
            } else {
                dirs |= 1;
                Ref c = r->left;
                if (!c) {
                    t = l;
                    l = l->parent;
                    break;
                }
                r = c;
            }
        }
        for (; h >= 0; --h) {
            if (!(dirs & 1)) {
                Ref p = l->parent;
                t = l->linkr(t);
                l = p;
            } else {
                Ref p = r->parent;
                t = r->linkl(t);
                r = p;
            }
            dirs >>= 1;
        }
        return t;
    }

    typedef std::pair<Ref, Ref> RefPair;

    // l＜t≦rの(l,r)に分割する
    RefPair split2(Ref t) {
        Ref p, l = t->left, r = t;
        Node::cut(l);
        t->linkl(NULL);
        while (p = t->parent) {
            t->parent = NULL;
            if (p->left == t)
                r = p->linkl(r);
            else
                l = p->linkr(l);
            t = p;
        }
        return RefPair(l, r);
    }
    // l＜t＜rの(l,t,r)に分割する。(l,r)を返す
    RefPair split3(Ref t) {
        Ref p, l = t->left, r = t->right;
        Node::cut(l), Node::cut(r);
        t->linklr(NULL, NULL);
        while (p = t->parent) {
            t->parent = NULL;
            if (p->left == t)
                r = p->linkl(r);
            else
                l = p->linkr(l);
            t = p;
        }
        return RefPair(l, r);
    }
    Ref cons(Ref h, Ref t) {
        assert(size(h) == 1);
        if (!t)
            return h;
        Ref u = NULL;
        while (true) {
            if (choiceRandomly(h, t)) {
                Ref p = t->parent;
                u = h->linkr(t);
                t = p;
                break;
            }
            Ref l = t->left;
            if (!l) {
                u = h;
                break;
            }
            t = l;
        }
        while (t) {
            u = t->linkl(u);
            t = t->parent;
        }
        return u;
    }
};

// free treeのために、辺を基本として扱う
class EulerTourTreeWithMarks {
    struct Node {
        typedef BottomupTreap<Node> BST;

        Node *left, *right, *parent;
        int size;
        unsigned priority;
        char marks, markUnions;  // 0ビット目がedgeMark, 1ビット目がvertexMark

        Node()
            : left(NULL),
              right(NULL),
              parent(NULL),
              size(1),
              priority(0),
              marks(0),
              markUnions(0) {}

        inline Node *update() {
            int size_t = 1, markUnions_t = marks;
            if (left) {
                size_t += left->size;
                markUnions_t |= left->markUnions;
            }
            if (right) {
                size_t += right->size;
                markUnions_t |= right->markUnions;
            }
            size = size_t, markUnions = markUnions_t;
            return this;
        }

        inline Node *linkl(Node *c) {
            if (left = c)
                c->parent = this;
            return update();
        }
        inline Node *linkr(Node *c) {
            if (right = c)
                c->parent = this;
            return update();
        }
        inline Node *linklr(Node *l, Node *r) {
            if (left = l)
                l->parent = this;
            if (right = r)
                r->parent = this;
            return update();
        }
        static Node *cut(Node *t) {
            if (t)
                t->parent = NULL;
            return t;
        }

        static const Node *findRoot(const Node *t) {
            while (t->parent)
                t = t->parent;
            return t;
        }
        static std::pair<Node *, int> getPosition(Node *t) {
            int k = BST::size(t->left);
            Node *p;
            while (p = t->parent) {
                if (p->right == t)
                    k += BST::size(p->left) + 1;
                t = p;
            }
            return std::make_pair(t, k);
        }
        static const Node *findHead(const Node *t) {
            while (t->left)
                t = t->left;
            return t;
        }
        static void updatePath(Node *t) {
            while (t) {
                t->update();
                t = t->parent;
            }
        }
        static int getPosition(const Node *t) {
            int k = t->left ? t->left->size : 0;
            while (1) {
                const Node *p = t->parent;
                if (!p)
                    return k;
                if (p->right == t)
                    k += (p->left ? p->left->size : 0) + 1;
                t = p;
            }
        }
    };

    typedef Node::BST BST;
    BST bst;

    std::vector<Node> nodes;
    //各頂点に対してその頂点から出ているarcを1つだけ代表として持つ(無い場合は-1)
    //逆にarcに対して対応する頂点はたかだか1つである
    std::vector<int> firstArc;
    //辺・頂点に対する属性
    std::vector<bool> edgeMark, vertexMark;

    inline int getArcIndex(const Node *a) const { return a - &nodes[0]; }

    inline int arc1(int ei) const { return ei; }
    inline int arc2(int ei) const { return ei + (numVertices() - 1); }

   public:
    inline int numVertices() const { return firstArc.size(); }
    inline int numEdges() const { return numVertices() - 1; }

    inline bool getEdgeMark(int a) const {
        return a < numEdges() ? edgeMark[a] : false;
    }
    inline bool getVertexMark(int v) const { return vertexMark[v]; }

   private:
    void updateMarks(int a, int v) {
        Node *t = &nodes[a];
        t->marks = getEdgeMark(a) << 0 | getVertexMark(v) << 1;
        Node::updatePath(t);
    }

    // firstArcの変更に応じて更新する
    void firstArcChanged(int v, int a, int b) {
        if (a != -1)
            updateMarks(a, v);
        if (b != -1)
            updateMarks(b, v);
    }

   public:
    class TreeRef {
        friend class EulerTourTreeWithMarks;
        const Node *ref;

       public:
        TreeRef() {}
        TreeRef(const Node *ref_) : ref(ref_) {}
        bool operator==(const TreeRef &that) const { return ref == that.ref; }
        bool operator!=(const TreeRef &that) const { return ref != that.ref; }
        bool isIsolatedVertex() const { return ref == NULL; }
    };

    void init(int N) {
        int M = N - 1;
        firstArc.assign(N, -1);
        nodes.assign(M * 2, Node());
        for (int i = 0; i < M * 2; i++)
            nodes[i].priority = bst.nextRand();
        edgeMark.assign(M, false);
        vertexMark.assign(N, false);
    }

    TreeRef getTreeRef(int v) const {
        int a = firstArc[v];
        return TreeRef(a == -1 ? NULL : Node::findRoot(&nodes[a]));
    }

    bool isConnected(int v, int w) const {
        if (v == w)
            return true;
        int a = firstArc[v], b = firstArc[w];
        if (a == -1 || b == -1)
            return false;
        return Node::findRoot(&nodes[a]) == Node::findRoot(&nodes[b]);
    }

    int getTreeFirstArc(TreeRef t) const {
        assert(!t.isIsolatedVertex());
        return getArcIndex(t.ref);
    }

    static int getSize(TreeRef t) {
        if (t.isIsolatedVertex())
            return 1;
        else
            return t.ref->size / 2 + 1;
    }

    int getPosition(int v) const {
        int a = firstArc[v];
        if (a == -1)
            return 0;
        return Node::getPosition(&nodes[a]);
    }

    void link(int ti, int v, int w) {
        int a1 = arc1(ti), a2 = arc2(ti);
        // v→wがa1に対応するようにする
        if (v > w)
            std::swap(a1, a2);

        int va = firstArc[v], wa = firstArc[w];

        Node *l, *m, *r;
        if (va != -1) {
            // evert。順番を入れ替えるだけ
            std::pair<Node *, Node *> p = bst.split2(&nodes[va]);
            m = bst.join(p.second, p.first);
        } else {
            // vが孤立点の場合
            m = NULL;
            firstArc[v] = a1;
            firstArcChanged(v, -1, a1);
        }
        if (wa != -1) {
            std::pair<Node *, Node *> p = bst.split2(&nodes[wa]);
            l = p.first, r = p.second;
        } else {
            // wが孤立点の場合
            l = r = NULL;
            firstArc[w] = a2;
            firstArcChanged(w, -1, a2);
        }
        // w→vの辺をmの先頭＝lの末尾にinsert
        m = bst.cons(&nodes[a2], m);
        // v→wの辺をmの末尾＝rの先頭にinsert
        r = bst.cons(&nodes[a1], r);

        bst.join(bst.join(l, m), r);
    }

    void cut(int ti, int v, int w) {
        // v→wがa1に対応するようにする
        if (v > w)
            std::swap(v, w);

        int a1 = arc1(ti), a2 = arc2(ti);
        std::pair<Node *, Node *> p = bst.split3(&nodes[a1]);
        int prsize = BST::size(p.second);
        std::pair<Node *, Node *> q = bst.split3(&nodes[a2]);
        Node *l, *m, *r;
        // a1,a2の順番を判定する。a1＜a2ならp.secondが変わっているはず
        if (p.second == &nodes[a2] || BST::size(p.second) != prsize) {
            l = p.first, m = q.first, r = q.second;
        } else {
            // a2＜a1の順番である。v→wの辺がa1であって親→子であることにする
            std::swap(v, w);
            std::swap(a1, a2);
            l = q.first, m = q.second, r = p.second;
        }

        // firstArcを必要に応じて書き換える
        if (firstArc[v] == a1) {
            int b;
            if (r != NULL) {
                // vが根じゃないなら右側の最初の辺でよい
                b = getArcIndex(Node::findHead(r));
            } else {
                // vが根なら最初の辺でよい。孤立点になるなら-1
                b = !l ? -1 : getArcIndex(Node::findHead(l));
            }
            firstArc[v] = b;
            firstArcChanged(v, a1, b);
        }
        if (firstArc[w] == a2) {
            // wが根になるので最初の辺でよい。孤立点になるなら-1
            int b = !m ? -1 : getArcIndex(Node::findHead(m));
            firstArc[w] = b;
            firstArcChanged(w, a2, b);
        }

        bst.join(l, r);
    }

    void changeEdgeMark(int ti, bool b) {
        assert(ti < numEdges());
        edgeMark[ti] = b;
        Node *t = &nodes[ti];
        t->marks = (b << 0) | (t->marks & (1 << 1));
        Node::updatePath(t);
    }
    void changeVertexMark(int v, bool b) {
        vertexMark[v] = b;
        int a = firstArc[v];
        if (a != -1) {
            Node *t = &nodes[a];
            t->marks = (t->marks & (1 << 0)) | (b << 1);
            Node::updatePath(t);
        }
    }

    template <typename Callback>
    bool enumMarkedEdges(TreeRef tree, Callback callback) const {
        return enumMarks<0, Callback>(tree, callback);
    }
    //孤立点の場合は呼び側でその頂点だけ処理する必要がある
    template <typename Callback>
    bool enumMarkedVertices(TreeRef tree, Callback callback) const {
        return enumMarks<1, Callback>(tree, callback);
    }

   private:
    // callback : TreeEdgeIndex×2 -> Bool
    //引数は頂点をそこからのincident arcで示し、"(正方向 ? 0 : N-1) +
    //treeEdgeIndex"を表す。方向はv,wの大小で処理すればよい
    // callbackは継続するかどうかをboolで返す。最後まで列挙し終えたかどうかを返す。
    template <int Mark, typename Callback>
    bool enumMarks(TreeRef tree, Callback callback) const {
        if (tree.isIsolatedVertex())
            return true;
        const Node *t = tree.ref;
        if (t->markUnions >> Mark & 1)
            return enumMarksRec<Mark, Callback>(t, callback);
        else
            return true;
    }

    //平衡木なので深さは深くないので再帰して問題ない
    template <int Mark, typename Callback>
    bool enumMarksRec(const Node *t, Callback callback) const {
        const Node *l = t->left, *r = t->right;
        if (l && (l->markUnions >> Mark & 1))
            if (!enumMarksRec<Mark, Callback>(l, callback))
                return false;
        if (t->marks >> Mark & 1)
            if (!callback(getArcIndex(t)))
                return false;
        if (r && (r->markUnions >> Mark & 1))
            if (!enumMarksRec<Mark, Callback>(r, callback))
                return false;
        return true;
    }

   public:
    //デバッグ用
    void debugEnumEdges(std::vector<int> &out_v) const {
        int M = numEdges();
        for (int ti = 0; ti < M; ti++) {
            const Node *t = &nodes[ti];
            if (t->left || t->right || t->parent)
                out_v.push_back(ti);
        }
    }
};

// treeEdgeにはそれぞれ0～N-1のインデックスが与えられる。これは全てのレベルで共通。
//ところで"level up"って和製英語なんだ。promoteでいいかな。
// Sampling heuristic ランダムケースで超速く(4倍とか)なったんだけど！いいね！
class HolmDeLichtenbergThorup {
    typedef HolmDeLichtenbergThorup This;
    typedef EulerTourTreeWithMarks Forest;
    typedef Forest::TreeRef TreeRef;

    int numVertices_m;
    int numSamplings;

    // DynamicTreeはコピーできないけどまあその状態で使わなきゃいいじゃんということで…
    std::vector<Forest> forests;

    std::vector<char> edgeLevel;
    std::vector<int> treeEdgeIndex;          // : EdgeIndex -> TreeEdgeIndex
    std::vector<int> treeEdgeMap;            // : TreeEdgeIndex -> EdgeIndex
    std::vector<int> treeEdgeIndexFreeList;  // : [TreeEdgeIndex]

    // arcも方向はEulerTourTreeと同じようにv,wの大小に合わせる
    std::vector<int> arcHead;

    std::vector<std::vector<int> > firstIncidentArc;
    std::vector<int> nextIncidentArc, prevIncidentArc;

    //一時的に使う。使い回して使う
    std::vector<bool> edgeVisited;
    std::vector<int> visitedEdges;  // : [EdgeIndex | TreeEdgeIndex]

    int arc1(int ei) const { return ei; }
    int arc2(int ei) const { return numMaxEdges() + ei; }
    int arcEdge(int i) const {
        return i >= numMaxEdges() ? i - numMaxEdges() : i;
    }

    bool replace(int lv, int v, int w) {
        Forest &forest = forests[lv];

        TreeRef vRoot = forest.getTreeRef(v), wRoot = forest.getTreeRef(w);
        assert(vRoot.isIsolatedVertex() || wRoot.isIsolatedVertex() ||
               vRoot != wRoot);

        int vSize = forest.getSize(vRoot), wSize = forest.getSize(wRoot);

        int u;
        TreeRef uRoot;
        int uSize;
        if (vSize <= wSize)
            u = v, uRoot = vRoot, uSize = vSize;
        else
            u = w, uRoot = wRoot, uSize = wSize;

        // replacement edgeを探す
        int replacementEdge = -1;
        enumIncidentArcs(forest, uRoot, u, lv,
                         FindReplacementEdge(uRoot, &replacementEdge));

        //"Sampling heuristic"
        //早い時点で見つかったならT_u,他のincident
        //arcsをレベルアップさせなくても計算量的に問題ない
        if (replacementEdge != -1 &&
            (int)visitedEdges.size() + 1 <= numSamplings) {
            // replacementEdgeを処理する
            deleteNontreeEdge(replacementEdge);
            addTreeEdge(replacementEdge);
            for (int i = 0; i < (int)visitedEdges.size(); i++)
                edgeVisited[visitedEdges[i]] = false;
            visitedEdges.clear();
            return true;
        }

        //見つけたincident
        //arcsを一斉にレベルアップさせる。edgeVisitedの後処理もする
        for (int i = 0; i < (int)visitedEdges.size(); i++) {
            int ei = visitedEdges[i];
            edgeVisited[ei] = false;

            deleteNontreeEdge(ei);

            ++edgeLevel[ei];

            insertNontreeEdge(ei);
        }
        visitedEdges.clear();

        //このレベルのT_uの辺を列挙する
        forest.enumMarkedEdges(uRoot, EnumLevelTreeEdges(this));
        //列挙したT_uの辺を一斉にレベルアップさせる
        for (int i = 0; i < (int)visitedEdges.size(); i++) {
            int ti = visitedEdges[i];

            int ei = treeEdgeMap[ti];
            int v = arcHead[arc2(ei)], w = arcHead[arc1(ei)];
            int lv = edgeLevel[ei];

            edgeLevel[ei] = lv + 1;

            forests[lv].changeEdgeMark(ti, false);
            forests[lv + 1].changeEdgeMark(ti, true);

            forests[lv + 1].link(ti, v, w);
        }
        visitedEdges.clear();

        if (replacementEdge != -1) {
            // T_uの辺列挙の前に構造が変わると困るのでreplacementEdgeはこのタイミングで処理する
            deleteNontreeEdge(replacementEdge);
            addTreeEdge(replacementEdge);
            return true;
        } else if (lv > 0) {
            return replace(lv - 1, v, w);
        } else {
            return false;
        }
    }

    struct EnumLevelTreeEdges {
        This *thisp;
        EnumLevelTreeEdges(This *thisp_) : thisp(thisp_) {}

        inline bool operator()(int a) {
            thisp->enumLevelTreeEdges(a);
            return true;
        }
    };
    void enumLevelTreeEdges(int ti) { visitedEdges.push_back(ti); }

    //孤立点の時特別な処理をするなどしなければいけないのでヘルパー
    template <typename Callback>
    bool enumIncidentArcs(Forest &forest,
                          TreeRef t,
                          int u,
                          int lv,
                          Callback callback) {
        if (t.isIsolatedVertex())
            return enumIncidentArcsWithVertex<Callback>(lv, u, callback);
        else
            return forest.enumMarkedVertices(
                t, EnumIncidentArcs<Callback>(this, lv, callback));
    }

    template <typename Callback>
    struct EnumIncidentArcs {
        This *thisp;
        int lv;
        Callback callback;

        EnumIncidentArcs(This *thisp_, int lv_, Callback callback_)
            : thisp(thisp_), lv(lv_), callback(callback_) {}

        inline bool operator()(int tii) const {
            return thisp->enumIncidentArcsWithTreeArc(tii, lv, callback);
        }
    };

    template <typename Callback>
    bool enumIncidentArcsWithTreeArc(int tii, int lv, Callback callback) {
        int u = getTreeEdgeIndexTailVertex(tii);
        return enumIncidentArcsWithVertex(lv, u, callback);
    }

    int getTreeEdgeIndexTailVertex(int tii) const {
        bool dir = tii >= numVertices() - 1;
        int ti = dir ? tii - (numVertices() - 1) : tii;
        int ei = treeEdgeMap[ti];
        int v = arcHead[arc2(ei)], w = arcHead[arc1(ei)];
        //方向を求め、そのarcのtailの頂点を取得する
        int u = !(dir != (v > w)) ? v : w;
        return u;
    }

    // 1つの頂点を処理する
    template <typename Callback>
    bool enumIncidentArcsWithVertex(int lv, int u, Callback callback) {
        int it = firstIncidentArc[lv][u];
        while (it != -1) {
            if (!callback(this, it))
                return false;
            it = nextIncidentArc[it];
        }
        return true;
    }

    struct FindReplacementEdge {
        TreeRef uRoot;
        int *replacementEdge;
        FindReplacementEdge(TreeRef uRoot_, int *replacementEdge_)
            : uRoot(uRoot_), replacementEdge(replacementEdge_) {}

        inline bool operator()(This *thisp, int a) const {
            return thisp->findReplacementEdge(a, uRoot, replacementEdge);
        }
    };

    // 1つのarcを処理する
    bool findReplacementEdge(int a, TreeRef uRoot, int *replacementEdge) {
        int ei = arcEdge(a);
        if (edgeVisited[ei])
            return true;

        int lv = edgeLevel[ei];
        TreeRef hRoot = forests[lv].getTreeRef(arcHead[a]);

        if (hRoot.isIsolatedVertex() || hRoot != uRoot) {
            //別の木に渡されているならreplacement edgeである。
            *replacementEdge = ei;
            return false;
        }
        // replacement edgeはvisitedEdgesに入れたくないのでこの位置でマークする
        edgeVisited[ei] = true;
        visitedEdges.push_back(ei);
        return true;
    }

    void addTreeEdge(int ei) {
        int v = arcHead[arc2(ei)], w = arcHead[arc1(ei)];
        int lv = edgeLevel[ei];

        int ti = treeEdgeIndexFreeList.back();
        treeEdgeIndexFreeList.pop_back();
        treeEdgeIndex[ei] = ti;
        treeEdgeMap[ti] = ei;

        forests[lv].changeEdgeMark(ti, true);

        for (int i = 0; i <= lv; i++)
            forests[i].link(ti, v, w);
    }

    void insertIncidentArc(int a, int v) {
        int ei = arcEdge(a);
        int lv = edgeLevel[ei];
        assert(treeEdgeIndex[ei] == -1);

        int next = firstIncidentArc[lv][v];
        firstIncidentArc[lv][v] = a;
        nextIncidentArc[a] = next;
        prevIncidentArc[a] = -1;
        if (next != -1)
            prevIncidentArc[next] = a;

        if (next == -1)
            forests[lv].changeVertexMark(v, true);
    }

    void deleteIncidentArc(int a, int v) {
        int ei = arcEdge(a);
        int lv = edgeLevel[ei];
        assert(treeEdgeIndex[ei] == -1);

        int next = nextIncidentArc[a], prev = prevIncidentArc[a];
        nextIncidentArc[a] = prevIncidentArc[a] = -2;

        if (next != -1)
            prevIncidentArc[next] = prev;
        if (prev != -1)
            nextIncidentArc[prev] = next;
        else
            firstIncidentArc[lv][v] = next;

        if (next == -1 && prev == -1)
            forests[lv].changeVertexMark(v, false);
    }

    void insertNontreeEdge(int ei) {
        int a1 = arc1(ei), a2 = arc2(ei);
        insertIncidentArc(a1, arcHead[a2]);
        insertIncidentArc(a2, arcHead[a1]);
    }

    void deleteNontreeEdge(int ei) {
        int a1 = arc1(ei), a2 = arc2(ei);
        deleteIncidentArc(a1, arcHead[a2]);
        deleteIncidentArc(a2, arcHead[a1]);
    }

   public:
    HolmDeLichtenbergThorup() : numVertices_m(0), numSamplings(0) {}

    int numVertices() const { return numVertices_m; }
    int numMaxEdges() const { return edgeLevel.size(); }

    void init(int N, int M) {
        numVertices_m = N;

        int levels = 1;
        while (1 << levels <= N / 2)
            levels++;

        //サンプリング数を設定する。適切な値はよくわからない
        numSamplings = (int)(levels * 1);

        forests.resize(levels);
        for (int lv = 0; lv < levels; lv++)
            forests[lv].init(N);

        edgeLevel.assign(M, -1);

        treeEdgeIndex.assign(M, -1);
        treeEdgeMap.assign(N - 1, -1);

        treeEdgeIndexFreeList.resize(N - 1);
        for (int ti = 0; ti < N - 1; ti++)
            treeEdgeIndexFreeList[ti] = ti;

        arcHead.assign(M * 2, -1);

        firstIncidentArc.resize(levels);
        for (int lv = 0; lv < levels; lv++)
            firstIncidentArc[lv].assign(N, -1);
        nextIncidentArc.assign(M * 2, -2);
        prevIncidentArc.assign(M * 2, -2);

        edgeVisited.assign(M, false);
    }

    bool insertEdge(int ei, int v, int w) {
        assert(0 <= ei && ei < numMaxEdges() && 0 <= v && v < numVertices() &&
               0 <= w && w < numVertices());
        assert(edgeLevel[ei] == -1);

        int a1 = arc1(ei), a2 = arc2(ei);
        arcHead[a1] = w, arcHead[a2] = v;

        bool treeEdge = !forests[0].isConnected(v, w);

        edgeLevel[ei] = 0;
        if (treeEdge) {
            addTreeEdge(ei);
        } else {
            treeEdgeIndex[ei] = -1;
            //ループは見たくないのでリストにも入れない
            if (v != w)
                insertNontreeEdge(ei);
        }

        return treeEdge;
    }

    bool deleteEdge(int ei) {
        assert(0 <= ei && ei < numMaxEdges() && edgeLevel[ei] != -1);

        int a1 = arc1(ei), a2 = arc2(ei);
        int v = arcHead[a2], w = arcHead[a1];

        int lv = edgeLevel[ei];
        int ti = treeEdgeIndex[ei];

        bool splitted = false;
        if (ti != -1) {
            treeEdgeMap[ti] = -1;
            treeEdgeIndex[ei] = -1;
            treeEdgeIndexFreeList.push_back(ti);

            for (int i = 0; i <= lv; i++)
                forests[i].cut(ti, v, w);

            forests[lv].changeEdgeMark(ti, false);

            splitted = !replace(lv, v, w);
        } else {
            //ループはリストに入ってない
            if (v != w)
                deleteNontreeEdge(ei);
        }

        arcHead[a1] = arcHead[a2] = -1;
        edgeLevel[ei] = -1;

        return splitted;
    }

    bool isConnected(int v, int w) const {
        return forests[0].isConnected(v, w);
    }

    int findComponentRepresent(int v) const {
        assert(0 <= v && v < numVertices());
        TreeRef t = forests[0].getTreeRef(v);
        if (t.isIsolatedVertex()) {
            return v;
        } else {
            int tii = forests[0].getTreeFirstArc(t);
            return getTreeEdgeIndexTailVertex(tii);
        }
    }

    bool getParity(int v) const { return forests[0].getPosition(v) % 2 != 0; }

    void dbgForestO(std::ostream &o) const {
        int N = numVertices(), M = numMaxEdges();
        int levels = forests.size();
        std::vector<bool> visited(M, false);
        o << "graph {\n";
        for (int lv = 0; lv < levels; lv++) {
            o << "subgraph cluster_" << lv << "{\n";
            o << "label = " << '"' << "level " << lv << '"' << ";\n";
            for (int u = 0; u < N; u++) {
                o << '"' << lv << "_" << u << '"';
                o << "[label=" << u << "];\n";

                int it = firstIncidentArc[lv][u];
                while (it != -1) {
                    int ei = arcEdge(it);
                    if (!visited[ei]) {
                        visited[ei] = true;
                        int v = arcHead[arc2(ei)], w = arcHead[arc1(ei)];
                        o << '"' << lv << "_" << v << '"' << " -- " << '"' << lv
                          << "_" << w << '"';
                        o << " [style=dotted]"
                          << ";\n";
                    }
                    it = nextIncidentArc[it];
                }
            }
            std::vector<int> edges;
            forests[lv].debugEnumEdges(edges);
            for (int i = 0; i < (int)edges.size(); i++) {
                int ti = edges[i];
                int ei = treeEdgeMap[ti];
                assert(ei != -1);
                int lvi = edgeLevel[ei];
                int a1 = arc1(ei), a2 = arc2(ei);
                int v = arcHead[a2], w = arcHead[a1];
                o << '"' << lv << "_" << v << '"' << " -- " << '"' << lv << "_"
                  << w << '"';
                //				o << " [style=" << (lvi == lv ?
                //"solid" : "dashed") << "]";
                o << ";\n";
            }
            o << "}\n";
        }
        o << "}" << std::endl;
    }
};

vector<int> sqrtSort(const vector<pair<int, int> > &q, const int S) {
    int n = q.size();
    vector<long long> pack(n);
    for (int i = 0; i < n; i++) {
        pack[i] =
            (long long)(q[i].first / S) << 50 |
            (long long)((q[i].first / S & 1) == 0 ? q[i].second
                                                  : (1 << 25) - 1 - q[i].second)
                << 25 |
            i;
    }
    sort(all(pack));
    vector<int> res(n);
    for (int i = 0; i < n; i++)
        res[i] = pack[i] & ((1 << 25) - 1);
    return res;
}

int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    vpii edges(m);
    rep(i, m) {
        int x, y;
        scanf("%d%d", &x, &y), --x, --y;
        edges[i] = mp(x, y);
    }
    cerr << endl;
    vector<int> maximal(m + 1, -1);
    HolmDeLichtenbergThorup hdt;
    hdt.init(n, m);
    int right = 0;
    rep(left, m + 1) {
        while (right < m) {
            int ei = right;
            pii p = edges[ei];
            int x = p.first, y = p.second;
            if (hdt.isConnected(x, y)) {
                //				hdt.dbgForestO(cerr);
                //				cerr << left << ", "<< right << ", "
                //<< x << ", "<< y << ": " << hdt.getParity(x) << ", " <<
                //hdt.getParity(y) << endl;
                if (hdt.getParity(x) == hdt.getParity(y))
                    break;
            }
            hdt.insertEdge(ei, x, y);
            ++right;
        }
        //		cerr << left << ": " << right << endl;
        maximal[left] = right;
        if (left == m)
            break;
        int ei = left;
        hdt.deleteEdge(ei);
    }

    rep(i, q) {
        int l, r;
        scanf("%d%d", &l, &r), --l;
        bool ans = maximal[l] >= r;
        puts(ans ? "Possible" : "Impossible");
    }

    return 0;
}
