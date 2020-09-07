#include <cassert>
using namespace std;

template <typename T>
class deque {
    private:
        int length;
        int capacity;
        int start;
        T* a;

    public:
        deque() : capacity(16), start(0), length(0), a(new T[16]) {}
        deque(int n) : capacity(n), start(0), length(0), a(new T[n]) {}
        ~deque() { delete[] a; }

        T pop_front() {
            assert(length);
            --length;
            T v = a[start++];
            if (start == capacity) start = 0;
            return v;
        }
        
        void push_front(T v) {
            if (length == capacity) {
                T* old = a;
                int new_capacity = capacity << 1;
                a = new T[new_capacity];
                for (int i = 0; i < length; ++i) {
                    a[i] = old[(i + start) % capacity];
                }
                delete[] old;
                capacity = new_capacity;
                start = 0;
            }
            start--;
            if (start < 0) start += capacity;
            a[start] = v;
            length++;
        }

        T pop_back() {
            assert(length);
            int idx = start + (--length);
            if (idx >= capacity) idx -= capacity;
            return a[idx];
        }
        
        void push_back(T v) {
            if (length == capacity) {
                T* old = a;
                int new_capacity = capacity << 1;
                a = new T[new_capacity];
                for (int i = 0; i < length; ++i) {
                    a[i] = old[(i + start) % capacity];
                }
                delete[] old;
                capacity = new_capacity;
                start = 0;
            }
            a[(start + length) % capacity] = v;
            ++length;
        }

        int size() { return length; }
};
