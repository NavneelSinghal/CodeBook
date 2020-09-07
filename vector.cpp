#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class vector {
    private:
        T* a;
        int capacity;
        int length;
    public:
        vector() : capacity(16), length(0), a(new T[16]) {}
        vector(int n) : capacity(n), length(n), a(new T[n]) {}
        vector(int n, T v) : capacity(n), length(n), a(new T[n]) {
            for (int i = 0; i < length; ++i) {
                a[i] = v;
            }
        }
        ~vector() { delete[] a; }
        
        void push_back(T v) {
            if (length == capacity) {
                T* old = a;
                capacity <<= 1;
                a = new T[capacity];
                for (int i = 0; i < length; ++i) {
                    a[i] = old[i];
                }
                delete[] old;
            }
            a[length++] = v;
        }

        // make sure length is not 0 while calling
        T pop_back() { return a[length-- - 1]; }
        
        // same as above
        T& back() { return a[length - 1]; }

        int size() { return length; }

        T& operator[](int index) {
            assert (0 <= index && index < length);
            return a[index];
        }
};

#if 0
int main() {
    vector<vector<int>> v(10);
    for (int i = 0; i < 10; ++i) {
        v.push_back(vector<int>(i));
    }
    cout << v.size() << '\n';
    for (int i = 0; i < v.size(); ++i) {
        v[i].push_back(i);
        for (int j = 0; j < v[i].size(); ++j)
            cout << v[i][j] << ' ';
        cout << '\n';
    }
    while (v.size()) {
        cout << "popping, size = " << v.size() << '\n';
        v.pop_back();
    }
    return 0;
}
#endif
