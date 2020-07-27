// note that this is slower than the usual priority queue if you're using
// dijkstra's algorithm, so only use if the memory O(m) is a bit too big for
// your purposes (like making an implicitly defined graph)

template <typename T, typename V = vector<T>, class S = less<T>>
class custom_priority_queue : public std::priority_queue<T, V, S> {
    public:
        bool remove(const T& value) {
            auto it = std::find(this->c.begin(), this->c.end(), value);
            if (it != this->c.end()) {
                this->c.erase(it);
                std::make_heap(this->c.begin(), this->c.end(), this->comp);
                return true;
            } else {
                return false;
            }
        }
};
