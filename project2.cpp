#include <list>
#include <unordered_set>
#include <vector>
#include <stack>
#include <queue>
#include <stdio.h>

#define INFINITE -1
#define max(a, b) ((a > b) ? (a) : (b))

using namespace std;

class Graph {
    private:
        int _numVertices;
        vector<vector<int>> _adjList;
    
    public:
        Graph(int V) : _numVertices(V), _adjList(V+1) {}

        void addEdge(int v, int w) {
            _adjList[v].push_back(w);
        }

        void DFS(int startVertex, vector<bool>& visited, stack<int>* endOrder) {
            stack<int> stack;
            int currentVertex;
            stack.push(startVertex);
            vector<bool> finished(_numVertices, false);
            while (!stack.empty()) {
                int end = true;
                currentVertex = stack.top();
                visited[currentVertex] = true;
                for (int neighbor : _adjList[currentVertex]) {
                    if (!visited[neighbor]) {
                        end = false;
                        stack.push(neighbor);
                    }
                }
                if (end) {
                    if (!finished[currentVertex]) {
                        endOrder->push(currentVertex);
                        finished[currentVertex] = true;
                    }
                    stack.pop();
                }
            }
        }

        stack<int> topologicalSort() {
            vector<bool> visited(_numVertices+1, false);
            stack<int> endOrder;
            for (int startVertex = 1; startVertex <= _numVertices; startVertex++) {
                if (!visited[startVertex]) {
                    DFS(startVertex, visited, &endOrder);
                }
            }
            return endOrder;
        }

        Graph buildDAG(vector<int> newVertex, int numVert) {
            Graph newGraph(numVert);
            for (int u = 1; u <= _numVertices; u++) {
                for (int neighbor : _adjList[u]) {
                    if (newVertex[u] != newVertex[neighbor]) {
                        newGraph.addEdge(newVertex[u], newVertex[neighbor]);
                    }
                }
            }
            return newGraph;
        }

        Graph findSCC(stack<int> topologicalOrder) {
            vector<bool> visited(_numVertices+1);
            vector<int> newVertex(_numVertices+1, -1);
            int newNumVertices = 0;
            while (!topologicalOrder.empty()) {
                vector<int> SCC;
                stack<int> endOrder;
                int startVertex = topologicalOrder.top();
                topologicalOrder.pop();
                DFS(startVertex, visited, &endOrder);
                bool newSCC = false;
                while (!endOrder.empty()) {
                    if (newVertex[endOrder.top()] == -1) {
                        newSCC = true;
                        newVertex[endOrder.top()] = newNumVertices;
                    }
                    endOrder.pop();
                }
                if (newSCC) {
                    newNumVertices++;
                }
            }
            Graph newGraph = buildDAG(newVertex, newNumVertices - 1);
            return newGraph;
        }

        int getMaxDistance() {
            vector<int> distance(_numVertices+1, 0);
            int maxDistance = 0;
            for (int u = _numVertices; u >= 0; u--) {
                for (int neighbor : _adjList[u]) {
                    distance[neighbor] = max(distance[neighbor], distance[u]+1);
                    if (distance[neighbor] > maxDistance) {
                        maxDistance = distance[neighbor];
                    }
                } 
            }
            return maxDistance;
        }

};

Graph* graph;
Graph* transpose;

void readInput() {
    int V, E, dump;
    dump = scanf("%d %d", &V, &E);
    graph = new Graph(V);
    transpose = new Graph(V);
    for (int i = 0; i < E; i++) {
        int v, w;
        dump = scanf("%d %d", &v, &w);
        graph->addEdge(v, w);
        transpose->addEdge(w, v);
    }
    (void)dump;
}

int main() {
    readInput();
    stack<int> topologicalOrder = graph->topologicalSort();
    Graph newGraph = transpose->findSCC(topologicalOrder);
    int res = newGraph.getMaxDistance();
    printf("%d\n", res);
    return 0;
}