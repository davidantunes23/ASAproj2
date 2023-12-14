#include <list>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <stack>
#include <stdio.h>

using namespace std;

typedef struct vertex {
    int _id;
    int _parent;
} Vertex;

class Graph {
    private:
        int _numVertices;
        list<int>* _adjList;
    
    public:
        Graph(int V) {
            _numVertices = V;
            _adjList = new list<int>[V+1];
        }

        void addEdge(int v, int w) {
            _adjList[v].push_back(w);
        }

        void collapse(list<int> path, int newVertex) {
            unordered_set<int> collapseList;
            while (path.back() != newVertex) {
                int last = path.back();
                collapseList.insert(last);
                for (int v : _adjList[last]) {
                    if (v != newVertex) {
                    _adjList[newVertex].push_back(v);
                    }
                }
                path.pop_back();
            }
            for (int i = 1; i <= _numVertices; i++) {
                int changed = 0;
                for (auto it = _adjList[i].begin(); it != _adjList[i].end();) {
                    if (collapseList.find(*it) != collapseList.end()) {
                        it = _adjList[i].erase(it);
                        changed = 1;
                    }
                    else {
                        ++it;
                    }
                }
                if (changed && i != newVertex) {
                    _adjList[i].push_back(newVertex);
                } 
            }
        }

        void DFS(int start) {
            unordered_set<int> visited;
            list<int> path;
            stack<Vertex> stack;
            Vertex startVertex = {._id = start, ._parent = -1};
            stack.push(startVertex);
            while (!stack.empty()) {
                Vertex currentVertex = stack.top();
                stack.pop();
                while (currentVertex._parent != path.back() && startVertex._id != currentVertex._id) {
                    path.pop_back();
                }
                if (visited.find(currentVertex._id) == visited.end()) {
                    visited.insert(currentVertex._id);
                    path.push_back(currentVertex._id);
                    printf("path: ");
                    for (int v : path) {
                        printf("%d ", v);
                    }
                    printf("\n");
                    for (int neighbor : _adjList[currentVertex._id]) {
                        if (visited.find(neighbor) == visited.end()) {
                            Vertex vertex = {._id = neighbor, ._parent = currentVertex._id};
                            stack.push(vertex);
                        }
                        else {
                            auto it = find(path.begin(), path.end(), neighbor);
                            if (it != path.end()) {
                                printf("loop found in %d!\n", neighbor);
                                collapse(path, neighbor);
                                return;
                            }
                        }
                    }
                }
            }
        }
};

Graph readInput() {
    int V, E;
    scanf("%d %d", &V, &E);
    Graph graph(V);
    for (int i = 0; i < E; i++) {
        int v, w;
        scanf("%d %d", &v, &w);
        graph.addEdge(v, w);
    }
    return graph;
}

int main() {
    Graph graph = readInput();
    graph.DFS(4);
    printf("novo:\n");
    graph.DFS(4);
    return 0;
}