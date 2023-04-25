/**Kattis - numberseteasy
 * UFDS + sieve of eratosthenes. The sieve is used to generate the primes up to 2000 (just something
 * bigger than b) then we just go along each prime and merge the sets of all the numbers that have
 * that prime as a factor (for primes >= p).
 * 
 * Time: O(n), Space: O(n) {number of primes under 2000 is constant}
 */
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;

ll _sieve_size;
bitset<10000010> bs;
vll primes;

// Able to handle 1e7 in < 1s
void sieve(ll upperbound = (ll)1e7)
{
    _sieve_size = upperbound + 1;
    bs.set();
    bs[0] = bs[1] = 0;
    for (ll i = 2; i <= _sieve_size; i++) {
        if (bs[i]) {
            for (ll j = i * i; j <= _sieve_size; j += i) {
                bs[j] = 0;
            }
            primes.push_back(i);
        }
    }
}
class UnionFind {  // OOP style
   private:
    vi p, rank, setSize;  // vi p is the key part
    int numSets;

   public:
    UnionFind(int N)
    {
        p.assign(N, 0);
        for (int i = 0; i < N; ++i) p[i] = i;
        rank.assign(N, 0);     // optional speedup
        setSize.assign(N, 1);  // optional feature
        numSets = N;           // optional feature
    }

    int findSet(int i) { return (p[i] == i) ? i : (p[i] = findSet(p[i])); }
    bool isSameSet(int i, int j) { return findSet(i) == findSet(j); }

    int numDisjointSets() { return numSets; }             // optional
    int sizeOfSet(int i) { return setSize[findSet(i)]; }  // optional

    void unionSet(int i, int j)
    {
        if (isSameSet(i, j)) return;         // i and j are in same set
        int x = findSet(i), y = findSet(j);  // find both rep items
        if (rank[x] > rank[y]) swap(x, y);   // keep x 'shorter' than y
        p[x] = y;                            // set x under y
        if (rank[x] == rank[y]) ++rank[y];   // optional speedup
        setSize[y] += setSize[x];            // combine set sizes at y
        --numSets;                           // a union reduces numSets
    }
};

int main()
{
    sieve(2000);
    int tc;
    cin >> tc;
    for (int t = 0; t < tc; t++) {
        int a, b, p;
        cin >> a >> b >> p;
        UnionFind uf(b - a + 1);

        for (int i = 0; i < (int)primes.size(); i++) {
            if (primes[i] >= p) {
                vector<int> hasp;
                for (int j = a; j <= b; j++) {
                    if (j % primes[i] == 0) {
                        hasp.push_back(j);
                    }
                }
                if (hasp.size() > 1) {
                    for (int j = 0; j < (int)hasp.size() - 1; j++) {
                        uf.unionSet(hasp[j] - a, hasp[j + 1] - a);
                    }
                }
            }
        }
        cout << "Case #" << t + 1 << ": " << uf.numDisjointSets() << "\n";
    }

    return 0;
}