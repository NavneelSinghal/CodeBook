#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"

namespace pbds {
#ifdef PB_DS_ASSOC_CNTNR_HPP
    template <class Key, typename... Hash>
    using unordered_set =
        __gnu_pbds::gp_hash_table<Key, __gnu_pbds::null_type, Hash...>;
    template <class Key, class Value, typename... Hash>
    using unordered_map = __gnu_pbds::gp_hash_table<Key, Value, Hash...>;
#endif
#ifdef PB_DS_TREE_POLICY_HPP
    template <typename T>
    using ordered_set =
        __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    template <typename T>
    using ordered_multiset =
        __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less_equal<T>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    template <class Key, class Value, class Compare = std::less<Key>>
    using ordered_map =
        __gnu_pbds::tree<Key, Value, Compare, __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>;
    // find_by_order(k)  returns iterator to kth element starting from 0;
    // order_of_key(k) returns count of elements strictly smaller than k;
    // for ordered_multiset, lower_bound and upper_bound swap roles
#endif
}  // namespace pbds

