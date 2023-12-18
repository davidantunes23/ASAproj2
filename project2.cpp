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
            vector<int> topologicalOrder(_numVertices);
            vector<bool> onStack(_numVertices+1, false);
            stack<int> L;
            
            for (int u = 1; u <= _numVertices; u++) {
                if (d[u] == INFINITE) {
                    Tarjan_Visit(u, d, low, L, onStack);
                }
            }
            return low;
        }

        void Tarjan_Visit(int u, vector<int>& d, vector<int>& low, stack<int>& L, vector<bool>& onStack) {
            static int visited = 0;
            d[u] = low[u] = ++visited;
            L.push(u);
            onStack[u] = true;
            for (int v : _adjList[u]) {
                if (d[v] == INFINITE) {
                    Tarjan_Visit(v, d, low, L, onStack);
                    low[u] = min(low[u], low[v]);
                }
                else if (onStack[v]) {
                    low[u] = min(low[u], low[v]);
                }
            }
            if (d[u] == low[u]) {
                while (L.top() != u) {
                    int poppedVertex = L.top();
                    L.pop();
                    onStack[poppedVertex] = false;
                }
                int poppedVertex = L.top();
                onStack[poppedVertex] = false;
            }
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

        int getMaxDistance(vector<int> topologicalOrder, vector<int> low) {
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
    for (int v : low) {
        printf("%d ", v);
    }
    printf("\n");
    *graph = graph->makeAcyclic(low); 
    vector<int> topologicalOrder = graph->topologicalSort();
    for (int v : topologicalOrder) {
        printf("%d ", v);
    }
    printf("\n");
    int res = graph->getMaxDistance(topologicalOrder, low);
    printf("%d\n", res);
    return 0;
}