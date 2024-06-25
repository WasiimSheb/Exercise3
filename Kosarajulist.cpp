#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <list>

using namespace std;

// Function to perform DFS and fill the stack
void fillOrder(int v, vector<bool>& visited, stack<int>& Stack, const list<list<int>>& adj) {
    visited[v] = true;
    auto it = next(adj.begin(), v);
    for (int neighbor : *it) {
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack, adj);
        }
    }
    Stack.push(v);
}

// Function to perform DFS on the transposed graph
void DFSUtil(int v, vector<bool>& visited, const list<list<int>>& transposedAdj, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    auto it = next(transposedAdj.begin(), v);
    for (int neighbor : *it) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited, transposedAdj, component);
        }
    }
}

// Function to get the transposed graph
list<list<int>> getTranspose(int n, const list<list<int>>& adj) {
    list<list<int>> transposedAdj(n);
    auto it = adj.begin();
    for (int v = 0; v < n; ++v, ++it) {
        for (int neighbor : *it) {
            auto transIt = next(transposedAdj.begin(), neighbor);
            transIt->push_back(v);
        }
    }
    return transposedAdj;
}

// Function to find and print all strongly connected components
void findSCCs(int n, int m, const vector<pair<int, int>>& edges) {
    list<list<int>> adj(n);
    auto it = adj.begin();
    for (const auto& edge : edges) {
        advance(it, edge.first - 1);
        it->push_back(edge.second - 1);
        it = adj.begin();  // Reset iterator
    }

    stack<int> Stack;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack, adj);
        }
    }

    list<list<int>> transposedAdj = getTranspose(n, adj);

    fill(visited.begin(), visited.end(), false);

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            vector<int> component;
            DFSUtil(v, visited, transposedAdj, component);
            cout << "SCC: ";
            for (int vertex : component) {
                cout << (vertex + 1) << " ";
            }
            cout << endl;
        }
    }
}

int main() {
    ifstream infile("graph.txt");
    if (!infile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }
    
    int n, m;
    infile >> n >> m;
    if (n <= 0 || m <= 0) {
        cerr << "Invalid number of vertices or edges" << endl;
        return 1;
    }
    
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; i++) {
        infile >> edges[i].first >> edges[i].second;
        if (edges[i].first <= 0 || edges[i].first > n || edges[i].second <= 0 || edges[i].second > n) {
            cerr << "Invalid edge: " << edges[i].first << " " << edges[i].second << endl;
            return 1;
        }
    }

    findSCCs(n, m, edges);

    return 0;
}
