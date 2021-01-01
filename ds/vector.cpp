#include <cassert>
#include <iostream>
using namespace std;

template <typename T>
class vector {
   private:
    T* a;
    int capacity;
    int length;

    void sort(T* a, int l, int r) {
        if (l >= r) return;
        vector<T> v(r - l + 1);
        int m = (l + r) >> 1;
        sort(a, l, m);
        sort(a, m + 1, r);
        int i = l, j = m + 1, k = 0;
        while (i <= m && j <= r) {
            if (a[i] <= a[j]) {
                v[k] = a[i];
                ++i, ++k;
            } else {
                v[k] = a[j];
                ++j, ++k;
            }
        }
        while (i <= m) {
            v[k] = a[i];
            ++i, ++k;
        }
        while (j <= r) {
            v[k] = a[j];
            ++j, ++k;
        }
        for (int i = l; i <= r; ++i) {
            a[i] = v[i - l];
        }
    }

   public:
    vector() : capacity(16), length(0), a(new T[16]) {}
    vector(int n) : capacity(n), length(n), a(new T[n]) {}
    vector(int n, T v) : capacity(n), length(n), a(new T[n]) {
        for (int i = 0; i < length; ++i) {
            a[i] = v;
        }
    }

    vector<T>(const vector<T>& v) {
        capacity = v.size();
        length = v.size();
        a = new T[capacity];
        for (int i = 0; i < length; ++i) {
            a[i] = v.get(i);
        }
    }
    ~vector() { delete[] a; }

    void push_back(T v) {
        if (length == capacity) {
            T* old = a;
            capacity <<= 1;
            a = new T[capacity];
            for (int i = 0; i < length; ++i) a[i] = old[i];
            delete[] old;
        }
        a[length] = v;
        ++length;
    }

    T pop_back() { return a[--length]; }

    T& back() { return a[length - 1]; }

    T get(int i) const { return a[i]; }

    int size() const { return length; }

    T& operator[](int index) { return a[index]; }

    void sort() { sort(a, 0, length - 1); }
};
