template<typename T>
struct Sieve{
    T n;
    vector<bool> is_prime;
    vector<T> primes;
    vector<T> spf;

    Sieve(T n) : n(n), is_prime(n + 1, true){
        this->is_prime[0] = this->is_prime[1] = false;
        for (T p = 2; p * p <= this->n; p++){
            if (this->is_prime[p]){
                for (T i = p * p; i <= this->n; i += p) this->is_prime[i] = false;
            }
        }
        for (T i = 2; i <= this->n; i++){
            if (this->is_prime[i]) this->primes.pb(i);
        }
    }

    bool query(T x){
        if (x < 0 || x > this->n) return false;
        return this->is_prime[x];
    }

    T count(T x){
        x = min(x, this->n);
        return (T)upper_bound(all(this->primes), x)-this->primes.begin();
    }

    void build_spf(){
        this->spf.assign(this->n + 1, 0);
        for (T i = 2; i <= this->n; i++){
            if (!this->spf[i]){
                for (T j = i; j <= this->n; j += i){
                    if (!this->spf[j]) this->spf[j] = i;
                }
            }
        }
    }

    map<T, int> factorize(T x){
        map<T, int> factors;
        while (x > 1){
            factors[this->spf[x]]++;
            x /= this->spf[x];
        }
        return factors;
    }
};
