#include <bits/stdc++.h>

using namespace std;

template <typename T>
struct Range {
    struct It {
        T i;
        const T skip;
        void operator++() { i += skip; }
        int operator*() const { return i; }
        bool operator!=(It o) const {
            return (skip >= 0) ? (i < *o) : (i > *o);
        }
    };
    const T l_, r_, skip_;
    Range(T l, T r, T skip = 1) : l_(l), r_(r), skip_(skip) {
        assert(skip != 0);
    }
    Range(T n) : Range(T(0), n, T(1)) {}
    It begin() const {
        return (skip_ >= 0) ? It{l_, skip_} : It{r_ - 1, skip_};
    }
    It end() const { return (skip_ >= 0) ? It{r_, skip_} : It{l_ - 1, skip_}; }
};

static int x;
void reset() { x = 0; }
int change() {
    x |= 1138717;
    x ^= (x << 11);
    x ^= (x >> 27) | (x << 1);
    return x;
}

int main() {
    int w1 = 0, w2 = 0;
    const int N = int(1e9) + 1;
    decltype(chrono::high_resolution_clock::now()) start, stop;
    decltype(chrono::duration_cast<chrono::microseconds>(stop - start)) duration;

    start = chrono::high_resolution_clock::now();
    reset();
    for (int i = N - 1; i >= 1; i -= 5) w1 += change();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << '\n';

    start = chrono::high_resolution_clock::now();
    reset();
    for (auto i : Range(1, N, -5)) w2 += change();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << '\n';
    assert(w1 == w2);
}
