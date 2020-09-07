#include <iostream>
#include <vector>
using namespace std;

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
}
