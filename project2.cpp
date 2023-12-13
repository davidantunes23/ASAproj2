#include <list>
#include <unordered_set>
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

        void DFS(int startVertex) {
            unordered_set<int> visited;
            stack<int> stack;
            stack.push(startVertex);
            while (!stack.empty()) {
                int currentVertex = stack.top();
                stack.pop();
                if (visited.find(currentVertex) == visited.end()) {
                    visited.insert(currentVertex);
                    printf("Visited %d\n", currentVertex);
                    for (int neighbor : _adjList[currentVertex]) {
                        if (visited.find(neighbor) == visited.end()) {
                            stack.push(neighbor);
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
    graph.DFS(3);
    return 0;
}