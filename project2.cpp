#include <list>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <stack>
#include <stdio.h>

using namespace std;

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

        void collapse(unordered_set<int> SCC) {
            int newVertex;
            for (int i = 1; i <= _numVertices; i++) {
                if (SCC.find(i) != SCC.end()) {
                    newVertex = i;
                    SCC.erase(i);
                    break;
                }
            }
            printf("Collapsing into %d: ", newVertex);
            for (int del : SCC) {
                printf("%d ", del);
                for (int v : _adjList[del]) {
                    if (v != newVertex) {
                        _adjList[newVertex].push_back(v);
                    }
                }
            }
            printf("\n");
            for (int i = 1; i <= _numVertices; i++) {
                int changed = 0;
                for (auto it = _adjList[i].begin(); it != _adjList[i].end();) {
                    if (SCC.find(*it) != SCC.end()) {
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

        void DFS(int startVertex, unordered_set<int>* visited, stack<int>* endOrder) {
            stack<int> stack;
            stack.push(startVertex);
            printf("DFS starting with %d\n", startVertex);
            while (!stack.empty()) {
                int currentVertex = stack.top();
                int end = 1;
                if (visited->find(currentVertex) == visited->end()) {
                    visited->insert(currentVertex);
                    printf("Visited %d\n", currentVertex);
                    for (int neighbor : _adjList[currentVertex]) {
                        if (visited->find(neighbor) == visited->end()) {
                            stack.push(neighbor);
                            end = 0;
                        }
                    }
                }
                if (end) {
                    endOrder->push(currentVertex);
                    stack.pop();
                }
            }
        }

        stack<int> topologicalOrder() {
            unordered_set<int> visited;
            stack<int> endOrder;
            for (int startVertex = 1; startVertex <= _numVertices; startVertex++) {
                if (visited.find(startVertex) == visited.end()) {
                    DFS(startVertex, &visited, &endOrder);
                }
            }
            return endOrder;
        }
        
        void makeAcyclic(stack<int> order) {
            printf("Removing cycles...\n");
            unordered_set<int> visited;
            while (!order.empty()) {
                unordered_set<int> SCC;
                stack<int> endOrder;
                int startVertex = order.top();
                order.pop();
                DFS(startVertex, &visited, &endOrder);
                while (!endOrder.empty()) {
                    SCC.insert(endOrder.top());
                    endOrder.pop();
                }
                if (SCC.size() > 1) {
                    collapse(SCC);
                }
                SCC.clear();
            }
        }
        
};

Graph* graph;
Graph* transposed;

void readInput() {
    int V, E;
    scanf("%d %d", &V, &E);
    graph = new Graph(V);
    transposed = new Graph(V);
    for (int i = 0; i < E; i++) {
        int v, w;
        scanf("%d %d", &v, &w);
        graph->addEdge(v, w);
        transposed->addEdge(w, v);
    }
}

int main() {
    readInput();
    stack<int> endOrder = graph->topologicalOrder();
    transposed->makeAcyclic(endOrder);
    transposed->topologicalOrder();
    return 0;
}