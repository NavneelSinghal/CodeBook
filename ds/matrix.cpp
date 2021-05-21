// matrix library

template <typename t>
struct matrix {
    int32_t rows, cols;
    vector<vector<t>> mat;

    // rows and columns
    matrix(int32_t r, int32_t c)
        : rows(r), cols(c), mat(vector<vector<t>>(r, vector<t>(c))){};

    // initializer list
    matrix(initializer_list<initializer_list<t>> l)
        : rows(l.size()), cols(l.begin()->size()) {
        mat = vector<vector<t>>(rows);
        int i = 0;
        for (auto& x : l) {
            mat[i] = x;
            ++i;
            assert(x.size() == cols);
        }
    };

    void fill(t val) {
        for (int32_t i = 0; i < rows; i++) {
            for (int32_t j = 0; j < cols; j++) {
                mat[i][j] = val;
            }
        }
    }

    void reset() { fill(0); }

    void setid() {
        assert(rows == cols);
        for (int32_t i = 0; i < rows; i++) {
            mat[i][i] = 1;
        }
    }

    static matrix id(int32_t n) {
        matrix m(n, n);
        m.setid();
        return m;
    }

    matrix operator+(const matrix& a) const {
        assert(rows == a.rows && cols == a.cols);
        matrix<t> res(rows, cols);
        for (int32_t i = 0; i < rows; i++) {
            for (int32_t j = 0; j < cols; j++) {
                res.mat[i][j] = mat[i][j] + a.mat[i][j];
            }
        }
    }

    matrix<t> operator*(const matrix<t>& a) const {
        assert(cols == a.rows);
        matrix<t> res(rows, a.cols);
        for (int32_t i = 0; i < rows; i++) {
            for (int32_t j = 0; j < a.cols; j++) {
                res.mat[i][j] = 0;
                for (int32_t k = 0; k < cols; k++) {
                    res.mat[i][j] += mat[i][k] * a.mat[k][j];
                }
            }
        }
        return res;
    }

    void operator+=(const matrix& a) { *this = *this + a; }
    void operator*=(const matrix& a) { *this = *this * a; }

    matrix<t> operator^(int32_t n) {
        matrix<t> ans(rows, cols);
        ans.setid();
        matrix<t> a = *this;
        while (n) {
            if (n & 1) ans *= a;
            a *= a;
            n >>= 1;
        }
        return ans;
    }
};

