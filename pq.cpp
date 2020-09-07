#include <iostream>
#include <cassert>
#include "vector.cpp"
using namespace std;

#if 0
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
                for (int i = 0; i < length; ++i) 
                    a[i] = old[i];
                delete[] old;
            }
            a[length++] = v;
        }

        T pop_back() { return a[length-- - 1]; }
        
        T& back() { return a[length - 1]; }
        
        int size() { return length; }
        
        T& operator[](int index) { return a[index]; }
};
#endif

// min heap
template <typename T>
class priority_queue {
    private:
        vector<T> a;
    
    public:
        void push(T v) {
            a.push_back(v);
            if (a.size() == 1) {
                return;
            }
            int cur = a.size() - 1;
            while (cur) {
                int parent = (cur - 1) >> 1;
                if (a[parent] > a[cur]) {
                    T temp = a[cur];
                    a[cur] = a[parent];
                    a[parent] = temp;
                    cur = parent;
                } else {
                    break;
                }
            }
        }

        T top() {
            assert(a.size());
            return a[0];
        }

        void pop() {
            assert(a.size());
            T ans = a[0];
            a[0] = a.pop_back();
            int here = 0;
            int left = 1;
            while (left < a.size()) {
                int right = left + 1;
                int minchild = left;
                T min = a[left];
                T h = a[here];
                if (right < a.size()) {
                    if (min > a[right]) {
                        minchild = right;
                        min = a[right];
                    }
                }
                if (h > min) {
                    a[here] = min;
                    a[minchild] = h;
                    here = minchild;
                    left = (minchild << 1) | 1;
                } else {
                    break;
                }
            }
        }

        int size() {
            return a.size();
        }

};

int main() {
    int n = 1000000;
    priority_queue<int> p;
    for (int i = 1; i < n; ++i) {
        int x = ((i << 1) * 18719383 | (i << 10)) % 12799;
        cout << "pushing " << x << "\n";
        p.push(x);
        cout << p.top() << '\n';
    }
    for (int i = 1; i < n; ++i) {
        cout << "popping " << p.top() << '\n';
        p.pop();
    }
}
