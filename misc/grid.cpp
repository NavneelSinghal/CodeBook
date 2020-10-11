// grid functions

int32_t n, m;

bool check(int32_t i, int32_t j) {
    return (i >= 0) && (i < n) && (j >= 0) && (j < m);
}

vector<pair<int32_t, int32_t>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

string direction = "DURL";
