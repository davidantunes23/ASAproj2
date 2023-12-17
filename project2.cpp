#include <list>
#include <unordered_set>
#include <vector>
#include <stack>
#include <stdio.h>

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
        
        void collapse(unordered_set<int> SCC) {
            printf("entrei\n");
            int newVertex = 0;
            for (int i = 1; i <= _numVertices; i++) {
                if (SCC.find(i) != SCC.end()) {
                    newVertex = i;
                    SCC.erase(i);
                    break;
                }
            }
            for (int del : SCC) {
                 printf("%d\n", del);
                for (int v : _adjList[del]) {
                    if (v != newVertex && SCC.find(v) != SCC.end()) {
                        _adjList[newVertex].insert(v);
                    }
                }
            }
            for (int i = 1; i <= _numVertices; i++) {
                int changed = 0;
                for (auto it = _adjList[i].begin(); it != _adjList[i].end();) {
                    if (SCC.find(*it) != SCC.end() || (*it) == newVertex) {
                        it = _adjList[i].erase(it);
                        changed = 1;
                    }
                    else {
                        ++it;
                    }
                }
                if (changed && i != newVertex && SCC.find(i) == SCC.end()) {
                    _adjList[i].insert(newVertex);
                } 
            }
        }

        int DFS(int startVertex, unordered_set<int>* visited, stack<int>* endOrder) {
            vector<int> distance(_numVertices+1);
            stack<int> stack;
            unordered_set<int> ended;
            int maxJumps = 0;
            distance[startVertex] = 0;
            stack.push(startVertex);
            printf("DFS em %d\n", startVertex);
            while (!stack.empty()) {
                int currentVertex = stack.top();
                int end = 1;
                if (visited->find(currentVertex) == visited->end()) {
                    printf("visitei %d\n", currentVertex);
                    visited->insert(currentVertex);
                    for (int neighbor : _adjList[currentVertex]) {
                        distance[neighbor] = max(distance[currentVertex] + 1, distance[neighbor]);
                        printf("dist %d = %d\n",neighbor, distance[neighbor]);
                        if (distance[neighbor] > maxJumps) {
                            maxJumps = distance[neighbor];
                        }
                        if (visited->find(neighbor) == visited->end()) {
                            stack.push(neighbor);
                            printf("encontrei %d\n", neighbor);
                            end = 0;
                        }
                    }
                }
                if (end && ended.find(currentVertex) == ended.end()) {
                    ended.insert(currentVertex);
                    endOrder->push(currentVertex);
                    printf("terminei %d\n", currentVertex);
                    stack.pop();
                }
                else if (end) {
                    stack.pop();
                }

            }
            return maxJumps;
        }

        int getMaxJumps(int startVertex, unordered_set<int>* visited) {
            vector<int> distance(_numVertices+1);
            stack<int> stack;
            unordered_set<int> ended;
            int maxJumps = 0;
            distance[startVertex] = 0;
            stack.push(startVertex);
            printf("DFS em %d\n", startVertex);
            while (!stack.empty()) {
                int currentVertex = stack.top();
                stack.pop();
                if (visited->find(currentVertex) == visited->end()) {
                    printf("visitei %d\n", currentVertex);
                    visited->insert(currentVertex);
                }
                for (int neighbor : _adjList[currentVertex]) {
                    distance[neighbor] = max(distance[currentVertex] + 1, distance[neighbor]);
                    printf("dist %d = %d\n",neighbor, distance[neighbor]);
                    if (distance[neighbor] > maxJumps) {
                        maxJumps = distance[neighbor];
                    }
                    stack.push(neighbor);
                    printf("encontrei %d\n", neighbor);
                }
            }
            return maxJumps;
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
            }
        }

        void computeMaxJumps() {
            unordered_set<int> visited;
            int maxJumps = 0, jumps = 0;
            for (int startVertex = 1; startVertex <= _numVertices; startVertex++) {
                if (visited.find(startVertex) == visited.end()) {
                    unordered_set<int> visitedTemp;
                    jumps = getMaxJumps(startVertex, &visitedTemp);
                    visited.insert(visitedTemp.begin(), visitedTemp.end());
                }
                if (jumps > maxJumps) {
                    maxJumps = jumps;
                }
            }
            printf("%d\n", maxJumps);
        }
};

Graph* graph;
Graph* transposed;

void readInput() {
    int V, E, dump;
    dump = scanf("%d %d", &V, &E);
    graph = new Graph(V);
    transposed = new Graph(V);
    for (int i = 0; i < E; i++) {
        int v, w;
        dump = scanf("%d %d", &v, &w);
        graph->addEdge(v, w);
        transposed->addEdge(w, v);
    }
    (void)dump;
}

int main() {
    readInput();
    stack<int> endOrder = graph->topologicalOrder();
    transposed->makeAcyclic(endOrder);
    transposed->computeMaxJumps();
    return 0;
}