/*
 * Time Complexity: Suffix Array: O(N + Character_Set_Size) time and space //
 128 --- ASCII
 *                  LCP: O(N) time and space
 * Usage:
 *       1. Suffix Array (returns s.size() elements, NOT considering
 0-length/empty suffix)
 *             auto sa = suffix_array(s); // s is the input string with ASCII
 characters
 *             auto sa_wide_char = suffix_array(s, LIM); // LIM = max(s[i]) + 2,
 s is the string with arbitary big characters.
 *       2. LCP:
 *            auto lcp = LCP(s, suffix_array(s)); // returns s.size() elements,
 where lcp[i]=LCP(sa[i], sa[i+1])
 * Status: Tested (DMOJ: ccc03s4, SPOJ: SARRAY (100pts), Yosupo's: Suffix Array
 & Number of Substrings, CodeForces EDU
 */
// Based on: Rickypon, https://judge.yosupo.jp/submission/10105
void induced_sort(const std::vector<int>& vec, int val_range,
                  std::vector<int>& SA, const std::vector<bool>& sl,
                  const std::vector<int>& lms_idx) {
    std::vector<int> l(val_range, 0), r(val_range, 0);
    for (int c : vec) {
        if (c + 1 < val_range) ++l[c + 1];
        ++r[c];
    }
    std::partial_sum(l.begin(), l.end(), l.begin());
    std::partial_sum(r.begin(), r.end(), r.begin());
    std::fill(SA.begin(), SA.end(), -1);
    for (int i = (int)lms_idx.size() - 1; i >= 0; --i)
        SA[--r[vec[lms_idx[i]]]] = lms_idx[i];
    for (int i : SA)
        if (i >= 1 && sl[i - 1]) SA[l[vec[i - 1]]++] = i - 1;
    std::fill(r.begin(), r.end(), 0);
    for (int c : vec) ++r[c];
    std::partial_sum(r.begin(), r.end(), r.begin());
    for (int k = (int)SA.size() - 1, i = SA[k]; k >= 1; --k, i = SA[k])
        if (i >= 1 && !sl[i - 1]) {
            SA[--r[vec[i - 1]]] = i - 1;
        }
}

std::vector<int> SA_IS(const std::vector<int>& vec, int val_range) {
    const int n = vec.size();
    std::vector<int> SA(n), lms_idx;
    std::vector<bool> sl(n);
    sl[n - 1] = false;
    for (int i = n - 2; i >= 0; --i) {
        sl[i] = (vec[i] > vec[i + 1] || (vec[i] == vec[i + 1] && sl[i + 1]));
        if (sl[i] && !sl[i + 1]) lms_idx.push_back(i + 1);
    }
    std::reverse(lms_idx.begin(), lms_idx.end());
    induced_sort(vec, val_range, SA, sl, lms_idx);
    std::vector<int> new_lms_idx(lms_idx.size()), lms_vec(lms_idx.size());
    for (int i = 0, k = 0; i < n; ++i)
        if (!sl[SA[i]] && SA[i] >= 1 && sl[SA[i] - 1]) {
            new_lms_idx[k++] = SA[i];
        }
    int cur = 0;
    SA[n - 1] = cur;
    for (size_t k = 1; k < new_lms_idx.size(); ++k) {
        int i = new_lms_idx[k - 1], j = new_lms_idx[k];
        if (vec[i] != vec[j]) {
            SA[j] = ++cur;
            continue;
        }
        bool flag = false;
        for (int a = i + 1, b = j + 1;; ++a, ++b) {
            if (vec[a] != vec[b]) {
                flag = true;
                break;
            }
            if ((!sl[a] && sl[a - 1]) || (!sl[b] && sl[b - 1])) {
                flag = !((!sl[a] && sl[a - 1]) && (!sl[b] && sl[b - 1]));
                break;
            }
        }
        SA[j] = (flag ? ++cur : cur);
    }
    for (size_t i = 0; i < lms_idx.size(); ++i) lms_vec[i] = SA[lms_idx[i]];
    if (cur + 1 < (int)lms_idx.size()) {
        auto lms_SA = SA_IS(lms_vec, cur + 1);
        for (size_t i = 0; i < lms_idx.size(); ++i) {
            new_lms_idx[i] = lms_idx[lms_SA[i]];
        }
    }
    induced_sort(vec, val_range, SA, sl, new_lms_idx);
    return SA;
}

std::vector<int> suffix_array(const std::string& s, const char first = 'a',
                         const char last = 'z') {
    std::vector<int> vec(s.size() + 1);
    std::copy(std::begin(s), std::end(s), std::begin(vec));
    for (auto& x : vec) x -= (int)first - 1;
    vec.back() = 0;
    auto ret = SA_IS(vec, (int)last - (int)first + 2);
    ret.erase(ret.begin());
    return ret;
}

// Author: https://codeforces.com/blog/entry/12796?#comment-175287
// Uses kasai's algorithm linear in time and space
std::vector<int> LCP(const std::string& s, const std::vector<int>& sa) {
    int n = s.size(), k = 0;
    std::vector<int> lcp(n), rank(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    for (int i = 0; i < n; i++, k ? k-- : 0) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
    }
    lcp[n - 1] = 0;
    return lcp;
}
