#include <list>
#include <unordered_set>
#include <vector>
#include <stack>
#include <stdio.h>

#define INFINITE -1
#define max(a, b) ((a > b) ? (a) : (b))

using namespace std;

class Graph {
    private:
        int _numVertices;
        vector<unordered_set<int>> _adjList;
    
    public:
        Graph(int V) : _numVertices(V), _adjList(V+1) {}

        void addEdge(int v, int w) {
            _adjList[v].insert(w);
        }
        
        void SCC_Tarjan() {
            vector<int> d(_numVertices+1, INFINITE);
            vector<int> low(_numVertices+1, -1);
            vector<bool> onStack(_numVertices+1, false);
            stack<int> L;

            for (int u = 1; u <= _numVertices; u++) {
                if (d[u] == INFINITE) {
                    Tarjan_Visit(u, d, low, L, onStack);
                }
            }
            for (int i = 1; i <= _numVertices; i++) {
                printf("low[%d] = %d\n", i, low[i]);
            }
            for (int i = 1; i <= _numVertices; i++) {
                printf("d[%d] = %d\n", i, d[i]);
            }
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
    graph->SCC_Tarjan();
    return 0;
}