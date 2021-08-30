// iterating over submasks
for (int mask = original;; mask = (mask - 1) & original) {
    // do something
    if (mask == 0) break;
}

// iterating over supermasks
for (int mask = original; mask < (1 << n); mask = (mask + 1) | original) {
    // do something
}
