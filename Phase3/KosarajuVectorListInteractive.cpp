#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <sstream>

using namespace std;

vector<list<int>> adj;
vector<list<int>> transposedAdj;
int n, m;

// Function to perform DFS and fill the stack
void fillOrder(int v, vector<bool>& visited, stack<int>& Stack) {
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack);
        }
    }
    Stack.push(v);
}

// Function to perform DFS on the transposed graph
void DFSUtil(int v, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : transposedAdj[v]) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited, component);
        }
    }
}

// Function to get the transposed graph
void getTranspose() {
    transposedAdj = vector<list<int>>(n);
    for (int v = 0; v < n; ++v) {
        for (int neighbor : adj[v]) {
            transposedAdj[neighbor].push_back(v);
        }
    }
}

// Function to find and print all strongly connected components
void findSCCs() {
    stack<int> Stack;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack);
        }
    }

    getTranspose();

    fill(visited.begin(), visited.end(), false);

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            vector<int> component;
            DFSUtil(v, visited, component);
            cout << "SCC: ";
            for (int vertex : component) {
                cout << (vertex + 1) << " ";
            }
            cout << endl;
        }
    }
}

void handleNewGraph(int vertices, int edges, istream& input) {
    n = vertices;
    m = edges;
    adj = vector<list<int>>(n);
    int u, v;
    for (int i = 0; i < m; ++i) {
        input >> u >> v;
        adj[u - 1].push_back(v - 1);
    }
    cout << "New graph created with " << n << " vertices and " << m << " edges." << endl;
}

void handleNewEdge(int u, int v) {
    adj[u - 1].push_back(v - 1);
    cout << "Edge added from " << u << " to " << v << "." << endl;
}

void handleRemoveEdge(int u, int v) {
    adj[u - 1].remove(v - 1);
    cout << "Edge removed from " << u << " to " << v << "." << endl;
}

int main() {
    string command;
    while (getline(cin, command)) {
        stringstream ss(command);
        string cmd;
        ss >> cmd;
        if (cmd == "Newgraph") {
            int vertices, edges;
            ss >> vertices >> edges;
            handleNewGraph(vertices, edges, cin);
        } else if (cmd == "Kosaraju") {
            findSCCs();
        } else if (cmd == "Newedge") {
            int u, v;
            ss >> u >> v;
            handleNewEdge(u, v);
        } else if (cmd == "Removeedge") {
            int u, v;
            ss >> u >> v;
            handleRemoveEdge(u, v);
        }
    }

    return 0;
}
