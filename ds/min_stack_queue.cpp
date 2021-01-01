// https://codeforces.com/blog/entry/53810
// https://cp-algorithms.com/data_structures/stack_queue_modification.html

// using cp-algorithms implementation
template <typename T>
struct MinStack {
    stack<pair<T, T>> stk;

    void push(const T new_element) {
        // new_min = min of the whole stack
        T new_minimum = new_element;
        if (!st.empty()) {
            new_minimum = min(new_minimum, stk.top().second);
        }
        stk.push({new_element, new_minimum});
    }

    void remove() { stk.pop(); }

    T top() { return stk.top().first; }

    T minimum() { return stk.top().second; }
};

template <typename T>
struct MinQueue1 {
    // same as the one in the codeforces link
    deque<T> q;

    T minimum() { return q.front(); }

    T push(const T new_element) {
        while (!q.empty() && q.back() > new_element) {
            q.pop_back();
        }
        q.push_back(new_element);
    }

    T remove(const T remove_element) {
        if (!q.empty() && q.front() == remove_element) {
            q.pop_front();
        }
    }
};

// TODO: complete

template <typename T>
struct MinQueue2 {
}

template <typename T>
struct MinQueue3 {
}
