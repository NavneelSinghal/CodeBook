// grid functions

int32_t n, m;

auto check = [&](int i, int j) {
    return (i >= 0) && (i < n) && (j >= 0) && (j < m);
};

vector<pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

string direction = "DURL";
