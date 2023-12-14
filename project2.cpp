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

        void DFS(int startVertex, unordered_set<int>* visited, stack<int>* endOrder) {
            stack<int> stack;
            stack.push(startVertex);
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
            printf("end order: ");
            while(!endOrder.empty()) {
                printf("%d ", endOrder.top());
                endOrder.pop();
            }
            printf("\n");
            return endOrder;
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
    return 0;
}