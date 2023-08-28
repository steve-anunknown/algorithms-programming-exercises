#include <iostream>
#include <tuple>
#include <vector>

using namespace std;
// total pairs, duplicates, loops.
typedef tuple<unsigned int, unsigned int ,unsigned int> info_t;
typedef vector < vector <unsigned int> > graph_t;

/// @brief find the parent (root) of a given node
/// @param pu the given node
/// @param parent the vector of parents
/// @return an integer (node) that is the parent (root).
unsigned int find(unsigned int pu, vector<unsigned int>& parent)
{
    while( parent[pu] != pu )
    {
        parent[pu] = parent[parent[pu]];
        pu = parent[pu];
    }
    return pu;
}

/// @brief check whether a given element exists in vector.
/// @param el the element which to search for.
/// @param arr the vector which to search inside of.
/// @return true if the element is found, false if it is not.
bool exists(const unsigned int& el, const vector<unsigned int>& arr)
{
    for (unsigned int i = 0; i < arr.size(); ++i)
    {
        if (el == arr[i]) return true;
    }
    return false;
}

const unsigned int cycle = 126;

info_t solve(const unsigned int& N, const unsigned int& F, vector<unsigned int>&p)
{
    float percentage = 0.0;         // used to store the biggest connected component percentage.
    unsigned int index = 0;         // used for indexing of the p array.
    unsigned int loops = 0;         // used for counting loops.
    unsigned int dups = 0;          // used for counting duplicates.
    unsigned int max_comp = 1;      // used to hold the size of the mzximum connected component.
    graph_t created(N);             // used to hold the formed pairs.
    vector<unsigned int> parent(N); // used to hold the parent of each node / disjoint set.
    vector<unsigned int> sizes(N,1); // used to hold the size of each node / disjoint set.
    for (unsigned int i = 0; i < N; ++i) parent[i] = i;
    while(true)
    {
        unsigned int pf = p[2*index % cycle];
        unsigned int ps = p[(2*index + 1) % cycle];
        if (pf > ps) swap(pf, ps);  // outer vector small element, inner vector big element.
        p[(2*index + 63) % cycle] = (p[(2*index + cycle) % cycle] + p[(2*index + 32 + cycle) % cycle]) % N;
        p[(2*index + 64) % cycle] = (p[(2*index + 1 + cycle) % cycle] + p[(2*index + 33 + cycle) % cycle]) % N;
        ++index;
        if ( pf == ps )
        {
            // check whether a loop has formed.
            ++loops;
            continue;
        }
        if ( exists(pf, created[ps]) )
        {
            // check whether the pair has
            // been created before.
            ++dups;
            continue;
        }
        // insert new pair into the vector.
        created[ps].push_back(pf);
        // find parents.
        unsigned int pu = find(pf, parent);
        unsigned int pv = find(ps, parent);
        // skip if they are in the same set.
        if (pu == pv) continue;
        else
        {
            // unite the sets otherwise.
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