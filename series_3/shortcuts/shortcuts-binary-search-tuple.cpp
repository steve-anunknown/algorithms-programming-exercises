// Author: Anastasios Stefanos Anagnostou 03119051
// School of Electrical and Computer Engineering
// National Technican University of Athens
// Algorithms and Complexity 7th Semester
// 2nd Exercise of 3rd set of programming exercises.

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <tuple>

using namespace std;
typedef unsigned long long bigint;
// an edge is a triplet (from, to , weight).
typedef tuple<uint, uint, bigint> edge;
// a graph is represented as a vector of vectors of edges.
typedef vector< vector<edge> > graph_type;

/// @brief make a duplicate of the graph and connect it.
/// @param graph graph to be duplicated.
/// @param current_size size of the graph.
/// @param original_size size of the copies which it consists of.
void duplicate(graph_type& graph, const uint& current_size, const uint& original_size)
{
    graph.resize(2*current_size);
    for (uint u = 0; u < current_size; ++u)
    {
        for (const auto& e: graph[u])
        {
            uint f = get<0>(e) + current_size;
            uint t = get<1>(e) + current_size;
            bigint w = get<2>(e);
            graph[f].push_back(make_tuple(f, t, w));
        }
    }
    for (uint u = current_size - original_size; u < current_size; ++u)
    {
        graph[u].reserve(2*graph[u].size());
        for (const auto& e: graph[u])
        {
            uint f = get<0>(e);
            uint t = get<1>(e) + original_size;
            bigint w = 0;
            graph[f].push_back(make_tuple(f, t, w));
        }
    }
}

/// @brief an implementation of the "greater" function for the pair type
struct Compare
{
    bool operator() (const pair<uint, uint> & lhs, const pair<uint, uint> & rhs) const
    {
        return lhs.first > rhs.first;
    }
};

/// @brief Minimum paths using dijkstra's algorithm
/// @param graph The graph for exploration
/// @param source The source node
/// @param target The target node
/// @param maxd The maximum acceptable distance of the source->targer path
/// @param distances The vector that stores the distances. Passed by reference so that it can be used later on in the main program
/// @return True if the distance is less than or equal to maxd. Otherwise false.
bool dijkstra(const graph_type& graph, const uint& source, const uint& target, const bigint& maxd, vector<uint>& distances, vector<bool>& visited)
{
    priority_queue<pair<uint, uint>, vector< pair<uint, uint> >, Compare> pq;
    distances[source] = 0;
    pq.push(make_pair(distances[source], source));
    while (!pq.empty())
    {
        uint u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (const auto &e : graph[u])
        {
            uint v = get<1>(e);
            bigint weight = get<2>(e);
            if (distances[u] + weight < distances[v])
            {
                distances[v] = distances[u] + weight;
                pq.push(make_pair(distances[v], v));
            }
        }
    }
    return distances[target] <= maxd;
}

/// @brief The function that solves the given problem.
/// @param graph The graph of the problem.
/// @param cities The number of vertices.
/// @param source The source node.
/// @param target The target node.
/// @param hours The maximum distance of the source->target path.
/// @return The minimum number of shortcuts to achieve the accepted distance.
int solve(graph_type & graph, const uint& cities, const uint& source, const uint& target, const uint& hours)
{
    // flag that is true at the first success of dijkstra
    bool flag = false;

    // exponential search for supremum of solutions
    uint upp = 1;
    // make the distances vector global in order to use
    // it later on and avoid running dijkstra again.
    vector<uint> distances(cities, numeric_limits<uint>::max());
    vector<bool> visited(cities, false);
    while(true)
    {
        flag = dijkstra(graph, source, target + (upp-1) * cities, hours, distances, visited);
        if (flag) break;
        duplicate(graph, (upp)*cities, cities);
        upp *=2;
        distances.resize(cities*(upp), numeric_limits<uint>::max());
        fill(distances.begin(), distances.end(), numeric_limits<uint>::max());
        visited.resize(cities*(upp), false);
        fill(visited.begin(), visited.end(), false);
    }
    // binary search for minimum
    uint low = 0;
    uint mid = 0;
    while(upp != low)
    {
        mid = (low + upp)/2;
        flag = (distances[target + mid * cities] <= hours);
        if (flag) upp = mid;
        else low = mid + 1;
    }
    // Check whether the last iteration failed or not.
    // If it failed, that means that the next would
    // have succeeded but did not manage to run,
    // therefore the answer is the supremum. If
    // it did succeed, then return the binary
    // search pointer.
    return ((!flag)? upp : mid); 
}

int main(void)
{
    uint cities, roads, source, target = 0;
    bigint hours = 0;
    cin >> cities >> roads >> source >> target >> hours;
    // decrease the indices of source and target
    // so that they are alligned with the vector's
    // indices.
    source--;
    target--;
    graph_type graph(cities);
    string str;
    vector<uint> nums;
    for (uint i = 0; i < roads; ++i)
    {
        uint f = 0;
        uint t = 0;
        bigint w = 0;
        cin >> f >> t >> w;
        // decrease the indices of the vertices
        // so that they are alligned with the
        // vector's indices.
        f--;
        t--;
        graph[f].push_back(make_tuple(f,t,w));
    }
    cout << solve(graph, cities, source, target, hours) << endl;
    return 0;
}