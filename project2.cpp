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
        
        vector<int> SCC_Tarjan() {
            vector<int> d(_numVertices+1, INFINITE);
            vector<int> low(_numVertices+1, -1);
            vector<bool> visited(_numVertices+1, false);
            int time = 1;
            
            for (int u = 1; u <= _numVertices; u++) {
                if (!visited[u]) {
                    vector<bool> onPath(_numVertices+1, false);
                    stack<int> stack;
                    stack.push(u);
                    while (!stack.empty()) {
                        int end = true;
                        int currentVertex = stack.top();
                        visited[currentVertex] = true;
                        onPath[currentVertex] = true;
                        if (d[currentVertex] == INFINITE) {
                            d[currentVertex] = time;
                            low[currentVertex] = time;
                            time++;
                        }
                        for (int neighbor : _adjList[currentVertex]) {
                            if (d[neighbor] == INFINITE) {
                                end = false;
                                stack.push(neighbor);
                            }
                            else if (low[currentVertex] > low[neighbor] && onPath[neighbor]){
                                end = false;
                                while (low[stack.top()] != low[neighbor]) {
                                    low[stack.top()] = low[neighbor];
                                    onPath[stack.top()] = false;
                                    stack.pop();
                                }
                            }
                        }
                        if (end) {
                            stack.pop();
                            onPath[currentVertex] = false;
                        }
                    }
                }
            }
            return low;
        }

        Graph makeAcyclic(vector<int> low) {
            Graph newGraph(_numVertices);
            for (int u = 1; u <= _numVertices; u++) {
                for (int neighbor : _adjList[u]) {
                    if (low[u] != low[neighbor]) {
                        newGraph.addEdge(low[u], low[neighbor]);
                    }
                }
            }
            return newGraph;
        }

        vector<int> topologicalSort() {
            vector<int> inDegree(_numVertices+1, 0);

            // Calculate in-degrees for all vertices
            for (int i = 1; i <= _numVertices; i++) {
                for (int neighbor : _adjList[i]) {
                    inDegree[neighbor]++;
                }
            }
            // Queue to store vertices with in-degree 0
            queue<int> q;
            // Initialize the queue with vertices having in-degree 0
            for (int i = 1; i <= _numVertices; i++) {
                if (inDegree[i] == 0) {
                    q.push(i);
                }
            }

            vector<int> result;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                result.push_back(u);

                // Reduce in-degree for all neighbors and enqueue if in-degree becomes 0
                for (int neighbor : _adjList[u]) {
                    if (--inDegree[neighbor] == 0) {
                        q.push(neighbor);
                    }
                }
            }
            return result;
        }

        int getMaxDistance(vector<int> topologicalOrder) {
            vector<int> distance(_numVertices+1, 0);
            int maxDistance = 0;
            for (int i = 0; i < _numVertices; i++) {
                int u = topologicalOrder[i];
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

void readInput() {
    int V, E, dump;
    dump = scanf("%d %d", &V, &E);
    graph = new Graph(V);
    for (int i = 0; i < E; i++) {
        int v, w;
        dump = scanf("%d %d", &v, &w);
        graph->addEdge(v, w);
    }
    (void)dump;
}

int main() {
    readInput();
    vector<int> low = graph->SCC_Tarjan();
    *graph = graph->makeAcyclic(low); 
    vector<int> topologicalOrder = graph->topologicalSort();
    int res = graph->getMaxDistance(topologicalOrder);
    printf("%d\n", res);
    return 0;
}