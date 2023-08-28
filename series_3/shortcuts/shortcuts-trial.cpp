// Author: Anastasios Stefanos Anagnostou 03119051
// School of Electrical and Computer Engineering
// National Technican University of Athens
// Algorithms and Complexity 7th Semester
// 2nd Exercise of 3rd set of programming exercises.

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;
typedef unsigned long long bigint;

// struct to represent a directed edge
struct edge
{
    uint from;
    uint to;
    bigint weight;
};

// a graph is represented as a vector of vectors
// there is a vector for each vertex
// and a vector element for every outgoing
// edge of that vertex
typedef vector< vector<edge*> > graph_type;

/// @brief Make a graph that consists of N copies of the given graph.
/// @param graph The given graph.
/// @param N The number of copies.
/// @return The mega-graph.
graph_type make_augmented_graph(const graph_type& graph, uint N)
{
    uint num_vertices = graph.size();
    // the augmented graph will have N*V vertices
    // given the V is the number of vertices of 
    // the original graph.
    graph_type augmented_graph(num_vertices * N);
    // first create N copies that are not connected
    // and then connect them.
    for (uint i = 0; i < N; ++i)
    {
        // if the original graph has V vertices
        // then the super-graph of N copies will
        // have V*N vertices. the i-th copy of 
        // the k-th vertex will be the k+i*v-th
        // vertex of the super-graph.
        uint offset = i * num_vertices;
        for (uint u = 0; u < num_vertices; ++u)
        {
            // iterate through the edges of each vertex
            // of the original graph.
            for (const auto& e : graph[u])
            {
                // for each edge of the original vertex
                // {k, m}, create a copy of the edge
                // {k+i*v, m+i*v} with the same weight.
                edge* new_edge = new edge;
                new_edge->from = e->from + offset;
                new_edge->to = e->to + offset;
                new_edge->weight = e->weight;
                augmented_graph[new_edge->from].push_back(new_edge);
            }
        }
    }
    // now connect the copies
    for (uint i = 1; i < N; ++i)
    {
        // keep track of the (i-1)-th and
        // the i-th copy of the graph
        uint prev = (i - 1) * num_vertices;
        uint curr = i * num_vertices;
        for (uint u = 0; u < num_vertices; ++u)
        {
            // iterate through the vertices
            // of the original graph
            for (const auto& e : graph[u])
            {
                // for each original edge, create
                // a copy of it that connects
                // the vertex of the (i-1)-th copy
                // with corresponding vertex of the
                // i-th copy and that has zero weight
                edge* new_edge = new edge;
                new_edge->from = e->from + prev;
                new_edge->to = e->to + curr;
                new_edge->weight = 0;
                augmented_graph[new_edge->from].push_back(new_edge);
            }
        }
    }
    return augmented_graph;
}

/// @brief releases the dynamically allocated memory of the graph
/// @param graph 
void free_augmented_graph(graph_type& graph)
{
    for (uint u = 0; u < graph.size(); ++u)
    {
        for (auto e: graph[u])
        {
            delete e;
        }
    }
}

/// @brief an implementation of the "greater" function for the pair type
struct Compare
{
    bool operator() (const pair<bigint, uint> & lhs, const pair<bigint, uint> & rhs) const
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
bool dijkstra(const graph_type& graph, const uint& source, const uint& target, const bigint& maxd, vector<bigint>& distances)
{
    priority_queue<pair<bigint, uint>, vector< pair<bigint, uint> >, Compare> pq;
    vector<bool> visited(graph.size(), false);
    distances[source] = 0;
    pq.push(make_pair(distances[source], source));
    while (!pq.empty())
    {
        uint u = pq.top().second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        for (auto e : graph[u])
        {
            uint v = e->to;
            bigint weight = e->weight;
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
/// @param roads The number of edges.
/// @param source The source node.
/// @param target The target node.
/// @param hours The maximum distance of the source->target path.
/// @return The minimum number of shortcuts to achieve the accepted distance.
int solve(const graph_type & graph, const uint & roads, const uint & source, const uint & target, const bigint & hours)
{
    // total number of cities in graph
    uint vertices = graph.size();


    // flag that is true at the first success of dijkstra
    bool flag = false;

    // exponential search for supremum of solutions
    graph_type big_graph = graph;
    uint upp = 1;
    // make the distances vector global in order to use
    // it later on and avoid running dijkstra again.
    vector<bigint> distances(vertices, numeric_limits<bigint>::max());
    while(true)
    {       
        big_graph = make_augmented_graph(graph, upp);
        flag = dijkstra(big_graph, source, target + (upp-1) * vertices, hours, distances);
        free_augmented_graph(big_graph);
        if (flag) break;
        upp *= 2;
        distances.resize(vertices*(upp), numeric_limits<bigint>::max());
        fill(distances.begin(), distances.end(), numeric_limits<bigint>::max());
    }


    // binary search for minimum
    uint low = 0;
    uint mid = 0;
    while(upp != low)
    {
        mid = (low + upp)/2;
        flag = (distances[target + mid * vertices] <= hours);
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
    for (uint i = 0; i < roads; ++i)
    {
        edge* new_edge = new edge;
        cin >> new_edge->from >> new_edge->to >> new_edge->weight;
        // decrease the indices of the vertices
        // so that they are alligned with the
        // vector's indices.
        (new_edge->from)--;
        (new_edge->to)--;
        graph[new_edge->from].push_back(new_edge);
    }
    cout << solve(graph, roads, source, target, hours) << endl;
    return 0;
}
