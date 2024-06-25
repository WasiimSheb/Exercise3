#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

// Function to perform DFS and fill the stack
void fillOrder(int v, vector<bool>& visited, stack<int>& Stack, const vector<vector<int>>& adj) {
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack, adj);
        }
    }
    Stack.push(v);
}

// Function to perform DFS on the transposed graph
void DFSUtil(int v, vector<bool>& visited, const vector<vector<int>>& transposedAdj, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : transposedAdj[v]) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited, transposedAdj, component);
        }
    }
}

// Function to get the transposed graph
vector<vector<int>> getTranspose(const vector<vector<int>>& adj) {
    size_t n = adj.size();
    vector<vector<int>> transposedAdj(n);
    for (size_t v = 0; v < n; ++v) {
        for (int neighbor : adj[v]) {
            transposedAdj[neighbor].push_back(v);
        }
    }
    return transposedAdj;
}

// Function to find and print all strongly connected components
void findSCCs(int n, int m, const vector<pair<int, int>>& edges) {
    vector<vector<int>> adj(n);
    for (const auto& edge : edges) {
        adj[edge.first - 1].push_back(edge.second - 1);
    }

    cout << "Graph built with adjacency list (vector of vectors)." << endl;

    stack<int> Stack;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack, adj);
        }
    }

    cout << "Finished filling order stack." << endl;

    vector<vector<int>> transposedAdj = getTranspose(adj);

    cout << "Transposed graph created." << endl;

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

    cout << "Input graph read successfully." << endl;

    findSCCs(n, m, edges);

    return 0;
}
