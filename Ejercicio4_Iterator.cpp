#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <memory>
#include <stdexcept>
using namespace std;
class Node {
private:
    int id;
public:
    explicit Node(int value):id(value) {}
    int getId() const {
        return id;
    }
};
class Graph {
private:
    unordered_map<int,Node> nodes;
    unordered_map<int,vector<int>> adjacency;
public:
    void addEdge(int from, int to) {
        nodes.emplace(from,Node(from));
        nodes.emplace(to,Node(to));
        adjacency[from].push_back(to);
    }
    const vector<int>& neighbors(int id) const {
        static const vector<int> empty;
        auto it = adjacency.find(id);
        if (it == adjacency.end())
            return empty;
        return it->second;
    }

    bool contains(int id) const {
        return nodes.find(id) != nodes.end();
    }
};
class GraphIterator {
public:
    virtual ~GraphIterator() = default;
    virtual bool hasNext() const = 0;
    virtual int next() = 0;
};
class BFSIterator:public GraphIterator {
private:
    const Graph& graph;
    queue<int> pending;
    unordered_set<int> visited;
public:
    BFSIterator(const Graph& g,int start):graph(g) {
        if (graph.contains(start)) {
            pending.push(start);
            visited.insert(start);
        }
    }
    bool hasNext() const override {
        return !pending.empty();
    }
    int next() override {
        if (!hasNext())
            throw out_of_range("BFSIterator no tiene mas nodos");
        int current=pending.front();
        pending.pop();
        for (int neighbor:graph.neighbors(current)) {
            if (visited.insert(neighbor).second)
                pending.push(neighbor);
        }
        return current;
    }
};
class DFSIterator:public GraphIterator {
private:
    const Graph& graph;
    stack<int> pending;
    unordered_set<int> visited;
public:
    DFSIterator(const Graph& g,int start):graph(g) {
        if (graph.contains(start)) {
            pending.push(start);
            visited.insert(start);
        }
    }
    bool hasNext() const override {
        return !pending.empty();
    }
    int next() override {
        if (!hasNext())
            throw out_of_range("DFSIterator no tiene mas nodos");
        int current=pending.top();
        pending.pop();
        const auto& adjacent=graph.neighbors(current);
        // Se insertan en orden inverso para visitar primero el vecino agregado primero.
        for (auto it=adjacent.rbegin();it!=adjacent.rend();++it) {
            if (visited.insert(*it).second)
                pending.push(*it);
        }
        return current;
    }
};
int main() {
    Graph g;
    g.addEdge(1,2);
    g.addEdge(1,3);
    g.addEdge(2,4);
    g.addEdge(3,4); // Arista adicional para probar que no se repiten nodos.
    unique_ptr<GraphIterator> it=make_unique<BFSIterator>(g,1);
    cout<<"BFS: ";
    while (it->hasNext())
        cout<<it->next()<<" ";
    cout<<"\n";
    // Cambiar el iterador en tiempo de ejecucion: ahora se recorre con DFS.
    it=make_unique<DFSIterator>(g,1);
    cout<<"DFS: ";
    while (it->hasNext())
        cout<<it->next()<<" ";
    cout<<"\n";
    return 0;
}