const int32_t mod = 1e9+7;
template<const int32_t MOD>
struct modular_int{
    int32_t x;
    static vector<int32_t> inverse_list;
    const static int32_t inverse_limit;
    modular_int() {x = 0;}
    template<typename T>
        modular_int(const T z) {x = (z % MOD); if (x < 0) x += MOD;}
    modular_int(const modular_int<MOD>* z) {x = z->x;}
    modular_int(const modular_int<MOD>& z) {x = z.x;}
    modular_int operator - (const modular_int<MOD>& m) const {modular_int<MOD> U; U.x = x - m.x; if (U.x < 0) U.x += MOD; return U;}
    modular_int operator + (const modular_int<MOD>& m) const {modular_int<MOD> U; U.x = x + m.x; if (U.x >= MOD) U.x -= MOD; return U;}
    modular_int& operator -= (const modular_int<MOD>& m) {x -= m.x; if (x < 0) x += MOD; return *this;}	
    modular_int& operator += (const modular_int<MOD>& m) {x += m.x; if (x >= MOD) x -= MOD; return *this;}
    modular_int operator * (const modular_int<MOD>& m) const {modular_int<MOD> U; U.x = (x* 1ull * m.x) % MOD; return U;}
    modular_int& operator *= (const modular_int<MOD>& m) {x = (x * 1ull * m.x) % MOD; return *this;}
    template<typename T>
        friend modular_int operator + (const T &l, const modular_int<MOD>& p) {return (modular_int<MOD>(l) + p);}
    template<typename T>
        friend modular_int operator - (const T &l, const modular_int<MOD>& p) {return (modular_int<MOD>(l) - p);}
    template<typename T>
        friend modular_int operator * (const T &l, const modular_int<MOD>& p) {return (modular_int<MOD>(l) * p);}
    template<typename T>
        friend modular_int operator / (const T &l, const modular_int<MOD>& p) {return (modular_int<MOD>(l) / p);}

    int32_t value() const {return x;}

    modular_int operator ^ (const modular_int<MOD>& cpower) const {modular_int<MOD> ans; ans.x = 1; modular_int<MOD> curr(this); int32_t power = cpower.x; while (power > 0) {if (power & 1) {ans *= curr;} power >>= 1; if (power) curr *= curr;} return ans;}
    modular_int operator ^ (long long power) const {if(x == 0 && power > 0) return modular_int(); modular_int<MOD> ans; ans.x = 1; modular_int<MOD> curr(this);/*Prime Mods*/ if (power >= MOD) {power %= (MOD - 1);} while (power > 0) {if (power & 1) {ans *= curr;} power >>= 1; if (power) curr *= curr;} return ans;}
    modular_int operator ^ (int32_t power) const {if(x == 0 && power > 0) return modular_int(); modular_int<MOD> ans; ans.x = 1; modular_int<MOD> curr(this); while (power > 0) {if (power & 1) {ans *= curr;} power >>= 1; if (power) curr *= curr;} return ans;}

    template<typename T>
        modular_int pow(T x) {return (*this)^x;}

    pair<long long,long long> gcd(const int32_t a, const int32_t b) const {if (b == 0) return {1, 0}; pair<long long,long long> c = gcd(b, a % b); return {c.second , c.first - (a/b)*c.second};}

    inline void init_inverse_list() const {vector<int32_t>& dp = modular_int<MOD>::inverse_list; dp.resize(modular_int<MOD>::inverse_limit + 1); int32_t n = modular_int<MOD>::inverse_limit; dp[0] = 1; if (n > 1) dp[1] = 1; for(int32_t i = 2; i <= n; ++i) {dp[i] = (dp[MOD % i] * 1ull *(MOD - MOD / i)) % MOD;}}

    modular_int<MOD> get_inv() const {if (modular_int<MOD>::inverse_list.size() < modular_int<MOD>::inverse_limit + 1) init_inverse_list(); if (this->x <= modular_int<MOD>::inverse_limit) {return modular_int<MOD>::inverse_list[this->x];} pair<long long,long long> G = gcd(this->x,MOD); return modular_int<MOD>(G.first);}

    modular_int<MOD> operator - () const {modular_int<MOD> v(0); v -= (*this); return v;}

    modular_int operator / (const modular_int<MOD>& m) const {modular_int<MOD> U(this); U *= m.get_inv(); return U;}
    modular_int& operator /= (const modular_int<MOD>& m) {(*this) *= m.get_inv(); return *this;}

    bool operator == (const modular_int<MOD>& m) const {return x == m.x;}
    bool operator < (const modular_int<MOD>& m) const {return x < m.x;}

    template<typename T>
        bool operator == (const T& m) const {return (*this) == (modular_int<MOD>(m));}
    template<typename T>
        bool operator < (const T& m) const {return x < (modular_int<MOD>(m)).x;}
    template<typename T>
        bool operator > (const T& m) const {return x > (modular_int<MOD>(m)).x;}
    template<typename T>
        friend bool operator == (const T& x, const modular_int<MOD>& m) {return (modular_int<MOD>(x)).x == m.x;}
    template<typename T> 
        friend bool operator < (const T& x, const modular_int<MOD>& m) {return (modular_int<MOD>(x)).x < m.x;}
    template<typename T>
        friend bool operator > (const T& x, const modular_int<MOD>& m) {return (modular_int<MOD>(x)).x > m.x;}
    friend istream& operator >> (istream& is, modular_int<MOD>& p) {int64_t val; is >> val; p.x = (val % MOD); if (p.x < 0) p.x += MOD; return is;}
    friend ostream& operator << (ostream& os, const modular_int<MOD>& p) {return os << p.x;}
};
using mint = modular_int<mod>;

template<const int32_t MOD>
vector<int32_t> modular_int<MOD>::inverse_list;
template<const int32_t MOD>
const int32_t modular_int<MOD>::inverse_limit = -1;
// template<>   //-> useful if computing inverse fact = i_f[i-1] / i;
// const int32_t modular_int<mod>::inverse_limit = 100000;
