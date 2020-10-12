// boring implementation

#if 0

Suppose we have a system of n equations

a \equiv a_i mod p_i, where p_i is a power of a prime

suppose all p_i's are different powers of primes // any general prob can be reduced to this after factorization and all - and look for contradictions as well

then we find x_1, x_2, ..., x_n where 
a = x_1 + x_2 p_1 + ... + x_n p_1 ... p_(n-1)

then

first compute r_ij = p_i^(-1) mod p_j

then do this:

for (int i = 0; i < k; ++i) {
    x[i] = a[i];
    for (int j = 0; j < i; ++j) {
        x[i] = r[j][i] * (x[i] - x[j]);

        x[i] = x[i] % p[i];
        if (x[i] < 0)
            x[i] += p[i];
    }
}

#endif
