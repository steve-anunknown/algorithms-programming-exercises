#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#define MAXN 50000
/*
        50.000
    x   50.000
    _______________
    2 500 000 000 | 2
                  --------------
                  1 250 000 000
                +        25 000
                ________________
                1 250 025 000 Bytes
                1,25 GB
*/


using namespace std;
// total pairs, duplicates, loops.
typedef tuple<unsigned int, unsigned int ,unsigned int> info_t;
typedef vector < vector <bool> > graph_t;

class SymmetricalMatrix
{
    private:
        // super space efficient
        // not so fast but it is no problem
        // because it isn't expected to
        // be accessed that much.
        vector<bool> data;
        unsigned int n;

        /// @brief Indexing function to map (i, j) to the corresponding index in data vector
        /// @param i row 
        /// @param j column
        /// @return adjusted index for correct iteration.
        unsigned int index(unsigned int i, unsigned int j) const
        {
            // Swap i and j if i > j
            if (i > j)
            {
                swap(i, j);
            }
            // Compute the index of the (i, j) element in the data vector
            return i * n - (i - 1) * i / 2 + j - i;
        }

    public:
        /// @brief Construct a symmetrical matrix
        /// @param size the number of rows or columns
        SymmetricalMatrix(unsigned int size)
        {
            n = size;
            data.resize(n * (n + 1) / 2, 0);
        }
        bool get(unsigned int i, unsigned int j)
        {
            return data[index(i, j)];
        }
        void set(unsigned int i, unsigned int j, bool value)
        {
            data[index(i, j)] = value;
        }
};


unsigned int find(unsigned int pu, vector<unsigned int>& parent)
{
    while( parent[pu] != pu)
    {
        parent[pu] = parent[parent[pu]];
        pu = parent[pu];
    }
    return pu;
}

const unsigned int cycle = 126;

info_t solve(const unsigned int& N, const unsigned int& F, vector<unsigned int>&p)
{
    float percentage = 0.0;         // used to store the biggest connected component percentage.
    unsigned int index = 0;         // used for indexing of the p array.
    unsigned int loops = 0;         // used for counting loops.
    unsigned int dups = 0;          // used for counting duplicates.
    unsigned int max_comp = 1;      // used to hold the size of the mzximum connected component.
    SymmetricalMatrix created(N);
    vector<unsigned int> parent(N); // used to hold the parent of each node / disjoint set.
    vector<unsigned int> sizes(N,1); // used to hold the size of each node / disjoint set.s
    for (unsigned int i = 0; i < N; ++i) parent[i] = i;
    while(true)
    {
        unsigned int pf = p[2*index % cycle];
        unsigned int ps = p[(2*index + 1) % cycle];
        p[(2*index + 63) % cycle] = (p[(2*index + cycle) % cycle] + p[(2*index + 32 + cycle) % cycle]) % N;
        p[(2*index + 64) % cycle] = (p[(2*index + 1 + cycle) % cycle] + p[(2*index + 33 + cycle) % cycle]) % N;
        ++index;
        if ( pf == ps )
        {
            ++loops;
            continue;
        }
        if ( created.get(pf, ps) )
        {
            ++dups;
            continue;
        }
        created.set(pf, ps, true);
        unsigned int pu = find(pf, parent);
        unsigned int pv = find(ps, parent);
        if (pu == pv) continue;
        else
        {
            if (sizes[pu] < sizes[pv]) swap(pu,pv);
            parent[pv] = pu;
            sizes[pu] += sizes[pv];
            max_comp = max(max_comp, sizes[pu]);
        }
        percentage = 100.0 * (max_comp*(1.0) / (N*1.0));
        if (percentage >= F) return make_tuple(index - dups - loops, dups, loops);
    }
}

int main(void)
{
    unsigned int N, F = 0;
    vector<unsigned int> p(cycle,0);
    cin >> N >> F;
    for (unsigned int i = 0; i < 63; ++i) cin >> p[i];
    info_t answer = solve(N, F, p);
    cout << get<0>(answer) << ' ' << get<1>(answer) << ' ' << get<2>(answer) << endl;
    return 0;
}