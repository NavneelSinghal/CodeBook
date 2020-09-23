// matrix library

template <typename T>
struct Matrix {
    
    int32_t rows, cols;
    vector<vector<T>> mat;
    
    // rows and columns
    Matrix(int32_t r, int32_t c) : rows(r), cols(c), mat(vector<vector<T>>(r, vector<T>(c))) {};
    
    // initializer list
    Matrix(initializer_list<initializer_list<T>> l) : rows(l.size()), cols(l.begin()->size()) {
        mat = vector<vector<T>>(rows);
        int i = 0;
        for (auto &x : l) {
            mat[i] = x;
            ++i;
            assert(x.size() == cols);
        }
    };
    
    void fill(T val) {
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
    
    static Matrix id(int32_t n) {
        Matrix m(n, n);
        m.setid();
        return m;
    }
    
    Matrix operator + (const Matrix& a) const {
        assert(rows == a.rows && cols == a.cols);
        Matrix<T> res(rows, cols);
        for (int32_t i = 0; i < rows; i++) {
            for (int32_t j = 0; j < cols; j++) {
                res.mat[i][j] = mat[i][j] + a.mat[i][j];
            }
        }
    }
    
    Matrix<T> operator * (const Matrix<T>& a) const {
        assert(cols == a.rows);
        Matrix<T> res(rows, a.cols);
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
    
    void operator += (const Matrix& a) { *this = *this + a; }
    void operator *= (const Matrix& a) { *this = *this * a; }

    Matrix<T> operator ^ (int32_t n) {
        Matrix<T> ans(rows, cols);
        ans.setid();
        Matrix<T> a = *this;
        while (n) {
            if (n & 1) ans *= a;
            a *= a;
            n >>= 1;
        }
        return ans;
    }
};
