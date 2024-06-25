
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <unordered_map>

using namespace std;

// Function to perform DFS and fill the stack
void fillOrder(int v, vector<bool>& visited, stack<int>& Stack, const unordered_map<int, list<int>>& adj) {
    visited[v] = true;
    for (int neighbor : adj.at(v)) {
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack, adj);
        }
    }
    Stack.push(v);
}

// Function to perform DFS on the transposed graph
void DFSUtil(int v, vector<bool>& visited, const unordered_map<int, list<int>>& transposedAdj, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : transposedAdj.at(v)) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited, transposedAdj, component);
        }
    }
}

// Function to get the transposed graph
unordered_map<int, list<int>> getTranspose(int n, const unordered_map<int, list<int>>& adj) {
    unordered_map<int, list<int>> transposedAdj;
    for (int i = 1; i <= n; i++) {
        for (int neighbor : adj.at(i)) {
            transposedAdj[neighbor].push_back(i);
        }
    }
    return transposedAdj;
}

// Function to find and print all strongly connected components
void findSCCs(int n, int m, const vector<pair<int, int>>& edges) {
    unordered_map<int, list<int>> adj;
    for (int i = 1; i <= n; i++) {
        adj[i] = list<int>();
    }

    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
    }

    stack<int> Stack;
    vector<bool> visited(n + 1, false);

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack, adj);
        }
    }

    unordered_map<int, list<int>> transposedAdj = getTranspose(n, adj);

    fill(visited.begin(), visited.end(), false);

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            vector<int> component;
            DFSUtil(v, visited, transposedAdj, component);
            for (int vertex : component) {
                cout << vertex << " ";
            }
            cout << endl;
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].first >> edges[i].second;
    }

    findSCCs(n, m, edges);

    return 0;
}
