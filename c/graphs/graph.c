#define N VERTICES
#define M EDGES

typedef struct {
    int v, next;
} edge;

edge g[2 * M + 1];
int head[N];
int cur_edge = 1;
void add_edge(int u, int v) {
    g[cur_edge] = (edge){u, head[v]};
    head[v] = cur_edge++;
}

// sample usage
// for (int it = head[u]; it; it = g[it].next) {
//      int v = tree[it].v;
