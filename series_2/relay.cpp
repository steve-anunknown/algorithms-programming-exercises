#include <iostream>
#include <algorithm>
#include <tuple>
#include <stack>
#include <vector>
#define MAXN 100000

using namespace std;
class node
{
    public:
        unsigned int id; // id of tree node.
        unsigned long p; // packing cost.
        unsigned long s; // seconds per km.
        unsigned long t_root; // minimum time to root.
        unsigned long d_root; // distance to root.
        unsigned long d_fath; // distance from father node.
        node* father; // father node.
        vector<node*> children; // list of children.
        vector<pair<node*, unsigned long>> neighbors; // list of neighbours with distances, temporal use.
        node(unsigned int i): id(i) {};

        /// @brief Calculate the minimum travel time from the node to the root.
        void SearchPath()
        {
            node* scout = this->father;
            unsigned long curr_dist = this->d_fath;
            unsigned long minimum = this->p + this->s * this->d_root;
            unsigned long temp = 0;
            while(scout)
            {
                temp = this->p + this->s * curr_dist + scout->t_root;
                if (temp < minimum)
                {
                    minimum = temp;
                    if ((this->s)*(scout->d_fath) > (scout->p)+(scout->s)*(scout->d_fath))
                        break;
                }
                curr_dist += scout->d_fath;
                scout = scout->father;
            }
            this->t_root =  minimum;
        }
};

node* nodes[MAXN] = {nullptr};  //statically allocated
bool checked[MAXN] = {false};   // statically allocated

/// @brief Create tree from the stored nodes iteratively.
/// @param root the root of the tree.
void CreateTreeIterative(node* root)
{
    stack<node*> notdone;
    notdone.push(root);
    node* curr = nullptr;
    node* c = nullptr;
    root->d_root = 0;
    while(!notdone.empty())
    {
        curr = notdone.top();
        notdone.pop();
        checked[curr->id -1] = true;
        for (auto p : curr->neighbors)
        {
            c = get<0>(p);
            if (!checked[c->id - 1])
            {
                curr->children.push_back(c);
                c->d_fath = get<1>(p);
                c->father = curr;
                c->d_root = c->d_fath + c->father->d_root;
                notdone.push(c);
            }
        }
    }
}

/// @brief Read data from file and create the nodes accordingly.
/// @return the pointer to the root node and the number of nodes in the tree.
pair<node*, unsigned long> ReadData()
{
    unsigned long N = 0;
    cin >> N;
    for (unsigned int line = 0; line < N-1; ++line)
    {
        unsigned int u = 0;
        unsigned int v = 0;
        unsigned long duv = 0;
        cin >> u >> v >> duv;
        --u; --v; //adjust indices so that they start from zero for the vector.
        if (!nodes[u]) nodes[u] = new node(u+1); // index+1 is the actual number that was read.
        if (!nodes[v]) nodes[v] = new node(v+1);
        nodes[u]->neighbors.push_back(make_pair(nodes[v], duv));
        nodes[v]->neighbors.push_back(make_pair(nodes[u], duv));
    }
    nodes[0]->p = 0;
    nodes[0]->s = 0;
    for (unsigned int line = 1; line <= N-1; ++line)
    {
        cin >> nodes[line]->p >> nodes[line]->s;
    }
    CreateTreeIterative(nodes[0]);
    return make_pair(nodes[0],N);
}

/// @brief Find the min distance of each node from root, using a depth first search iteratively implemented.
/// @param n the root.
/// @param answer the vector that holds the answers.
void DfsIterative(node* n, vector<unsigned long>& answer)
{
    if (!n) return;
    stack<node*> notdone;
    notdone.push(n);
    node* curr = nullptr;
    while (!notdone.empty())
    {
        curr = notdone.top();
        notdone.pop();
        for (auto c: curr->children)
        {
            c->SearchPath();
            answer[c->id-2] = c->t_root;
            notdone.push(c);
        }
    }
}


/// @brief Solve the problem for the given tree.
/// @param root The root of the tree.
/// @param size The amount of tree nodes.
/// @return A vector of unsigned integers, that is the min time to travel from a node to the root.
vector<unsigned long int> solve(node* root, unsigned int size)
{
    if (!root) exit(1);
    vector<unsigned long int> answer(size-1);
    node* curr = root;
    root->d_fath = 0;
    root->t_root = 0;
    DfsIterative(root, answer);
    return answer;
}

int main(void)
{
    unsigned int N = 0;
    node* tree = nullptr;
	tie(tree, N) = ReadData();
    vector<unsigned long int> answer = solve(tree, N);
    for (unsigned int j = 0; j < answer.size(); ++j)
    {
        cout << answer[j] << ((j == answer.size()-1) ? '\n': ' ');
    }
    return 0;
}
