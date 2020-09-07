#include <iostream>

template <typename T, typename V>
class pair {
    public:
        T F;
        V S;
        bool operator < (const pair<T, V> p) {
            if (F < p.F) return true;
            if (F == p.F) return S < p.S;
            return false;
        }
        bool operator == (const pair<T, V> p) {
            return (F == p.F) && (S == p.S);
        }
};

int main() {
    return 0;
}
