// Author: Anastasios Stefanos Anagnostou 03119051
// School of Electrical and Computer Engineering
// National Technican University of Athens
// Algorithms and Complexity 7th Semester
// 2nd Exercise of 3rd set of programming exercises.

#include <iostream>
#include <vector>
#include <queue>

#define MAXN 1000
#define MAXM 10000
#define MAXB 1000000000
#define MAXL 1000000

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

graph_type add_copy(graph_type& graph1, const graph_type& original)
{
    uint vertices1 = graph1.size();
    uint vertices2 = original.size();
    graph1.resize(vertices1+vertices2);
    for (uint u = 0; u < vertices2; ++u)
    {
        for (const auto& e: original[u])
        {
            edge* new_edge = new edge;
            new_edge->from = e->from + vertices1 - vertices2;
            new_edge->to = e->to + vertices1;
            new_edge->weight = 0;
            graph1[new_edge->from].push_back(new_edge);
            edge* new_edge2 = new edge;
            new_edge2->from = e->from + vertices1;
            new_edge2->to = e->to + vertices1;
            new_edge2->weight = e->weight;
            graph1[new_edge2->from].push_back(new_edge2);
        }
    }
    return graph1;
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
/// @param max_distance The maximum acceptable distance of the source->targer path
/// @return True if the distance is less than or equal to max_distance. Otherwise false.
bool dijkstra(const graph_type& graph, const int& source, const int& target, const bigint& max_distance)
{
    priority_queue<pair<bigint, uint>, vector< pair<bigint, uint> >, Compare> pq;
    vector<bigint> distances(graph.size(), MAXB+1);
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
    return distances[target] <= max_distance;
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
    // numbers of roads to be shortcut
    uint shortcuts = 0;
    // total number of cities in graph
    uint vertices = graph.size();
    // flag that is true at the first success of dijkstra
    bool flag = false;
    graph_type big_graph = graph;
    flag = dijkstra(big_graph, source, target, hours);
    if (flag) return 0;
    for (shortcuts = 1; shortcuts < roads; ++shortcuts)
    {
        add_copy(big_graph, graph);
        flag = dijkstra(big_graph, source, target + shortcuts * vertices, hours);
        if (flag) break;
    }
    free_augmented_graph(big_graph);
    return shortcuts; 
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