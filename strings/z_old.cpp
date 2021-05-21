// z[i] is the length of the longest common prefix between s and the suffix of s
// starting at i.
vector<int> z_function(string s) {
    int n = (int)s.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}

/*


   https://codeforces.com/group/3qadGzUdR4/contest/101710

   z to kmp

// naive:
// assuming z[0] = n
vector<int> pi(n);
for (int i = 1; i < n; ++i) {
    for (int k = 0; k < z[i]; ++k) {
        remax(pi[i + k], k + 1);
    }
}

// so pi[j] = max(0, 1 + j - (min i such that 0 <= i < n and i <= j < i + z[i]))

// so we do this (in pi[i], we store the min such i if it exists)
vector<int> pi(n, INF);
int j = 0; // current j
for (int i = 0; i < n; ++i) {
    int j_max = i + z[i];
    for (remax(j, i); j < j_max; ++j) {
        pi[j] = i;
    }
}
// since j keeps increasing everywhere, we do only O(n) iterations
// looking at a diagram with intervals of [i, i + z[i]) helps to understand the above loop
pi[0] = 0
for (int i = 1; i < n; ++i) {
    pi[i] = max(0, i + 1 - pi[i]);
}

Easier way to implement:

pi[0] = 0;
for (int i = 1; i < n; ++i) {
    for (int j = z[i] - 1; j >= 0 && !pi[i + j]; --j) {
        pi[i + j] = j + 1;
    }
}

    kmp to z

// naive:
vector<int> z(n);
z[0] = n;
for (int i = 1; i < n; ++i) {
    
}


   */
