#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <algorithm>

using namespace std;

vector<list<int>> adj;
vector<list<int>> transposedAdj;
int n, m;
fd_set master;

void fillOrder(int v, vector<bool>& visited, stack<int>& Stack) {
    visited[v] = true;
    for (int neighbor : adj[v]) {
        if (!visited[neighbor]) {
            fillOrder(neighbor, visited, Stack);
        }
    }
    Stack.push(v);
}

void DFSUtil(int v, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int neighbor : transposedAdj[v]) {
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited, component);
        }
    }
}

void getTranspose() {
    transposedAdj = vector<list<int>>(n);
    for (int v = 0; v < n; ++v) {
        for (int neighbor : adj[v]) {
            transposedAdj[neighbor].push_back(v);
        }
    }
}

string findSCCs() {
    stack<int> Stack;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            fillOrder(i, visited, Stack);
        }
    }

    getTranspose();

    fill(visited.begin(), visited.end(), false);

    stringstream ss;

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            vector<int> component;
            DFSUtil(v, visited, component);
            ss << "SCC: ";
            for (int vertex : component) {
                ss << (vertex + 1) << " ";
            }
            ss << endl;
        }
    }

    return ss.str();
}

void handleNewGraph(int vertices, int edges, int client_fd) {
    n = vertices;
    m = edges;
    adj = vector<list<int>>(n);
    char buf[256];
    int u, v;

    for (int i = 0; i < m; ++i) {
        memset(buf, 0, sizeof(buf));
        int nbytes = recv(client_fd, buf, sizeof(buf) - 1, 0);
        if (nbytes <= 0) {
            if (nbytes == 0) {
                cout << "Socket " << client_fd << " hung up" << endl;
            } else {
                perror("recv");
            }
            close(client_fd);
            FD_CLR(client_fd, &master);
            return;
        }
        buf[nbytes] = '\0';
        stringstream ss(buf);
        ss >> u >> v;
        if (u < 1 || u > n || v < 1 || v > n) {
            cerr << "Invalid edge: " << u << " " << v << endl;
            --i; // Retry the current edge
            continue;
        }
        adj[u - 1].push_back(v - 1);
    }
    cout << "Graph with " << n << " vertices and " << m << " edges created." << endl;
}

void handleNewEdge(int u, int v) {
    if (u < 1 || u > n || v < 1 || v > n) {
        cerr << "Invalid edge: " << u << " " << v << endl;
        return;
    }
    adj[u - 1].push_back(v - 1);
    cout << "Edge added: " << u << " -> " << v << endl;
}

void handleRemoveEdge(int u, int v) {
    if (u < 1 || u > n || v < 1 || v > n) {
        cerr << "Invalid edge: " << u << " " << v << endl;
        return;
    }
    adj[u - 1].remove(v - 1);
    cout << "Edge removed: " << u << " -> " << v << endl;
}

string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main() {
    fd_set read_fds;
    int fdmax, listener, newfd, nbytes;
    struct sockaddr_in myaddr, remoteaddr;
    socklen_t addrlen;
    char buf[1024];
    int yes = 1;
    char remoteIP[INET6_ADDRSTRLEN];
    int port = 9034;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(port);
    memset(&(myaddr.sin_zero), '\0', 8);

    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(1);
    }

    FD_SET(listener, &master);
    fdmax = listener;

    while (true) {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) {
                            fdmax = newfd;
                        }
                        cout << "New connection from "
                             << inet_ntop(remoteaddr.sin_family, &remoteaddr.sin_addr, remoteIP, INET6_ADDRSTRLEN)
                             << " on socket " << newfd << endl;
                    }
                } else {
                    if ((nbytes = recv(i, buf, sizeof(buf) - 1, 0)) <= 0) {
                        if (nbytes == 0) {
                            cout << "Socket " << i << " hung up" << endl;
                        } else {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        buf[nbytes] = '\0';
                        string command(buf);
                        stringstream ss(command);
                        string cmd;
                        ss >> cmd;
                        cmd = toLowerCase(cmd);  // Convert command to lowercase
                        string response;
                        if (cmd == "newgraph") {
                            int vertices, edges;
                            ss >> vertices >> edges;
                            response = "Send the edges.\n";
                            send(i, response.c_str(), response.length(), 0);
                            handleNewGraph(vertices, edges, i);
                            response = "New graph created.\n";
                            send(i, response.c_str(), response.length(), 0);
                        } else if (cmd == "kosaraju") {
                            response = findSCCs();
                            send(i, response.c_str(), response.length(), 0);
                        } else if (cmd == "newedge") {
                            int u, v;
                            ss >> u >> v;
                            handleNewEdge(u, v);
                            response = "Edge added.\n";
                            send(i, response.c_str(), response.length(), 0);
                        } else if (cmd == "removeedge") {
                            int u, v;
                            ss >> u >> v;
                            handleRemoveEdge(u, v);
                            response = "Edge removed.\n";
                            send(i, response.c_str(), response.length(), 0);
                        } else {
                            response = "Invalid command.\n";
                            send(i, response.c_str(), response.length(), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
