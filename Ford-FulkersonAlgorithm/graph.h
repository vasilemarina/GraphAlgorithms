#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <stack>
#include <unordered_set>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>

class Graph
{
public:
    Graph(int nodeRadius);
    ~Graph();
    void addNode(QPoint p);
    void addEdge(Node* first, Node* second, QWidget* parent);
    void addEdge(Node* first, Node* second, int cost=0);
    bool dfs(Node* start, Node* end, std::vector<bool>& visited, std::vector<Node*>& parent) const;
    int getMaxFlow(Node* start, Node* end, Graph&);
    void makeEdgeMap();
    std::vector<Node*>& getNodes();
    std::vector<Edge>& getEdges();

private:
    void getResidualGraph(Graph& );
    void findMinCut(Graph& residualGraph, Node* startNode) const;

    struct pair_hash {
        std::size_t operator()(const std::pair<int, int>& p) const;
    };

    int m_nodeRadius;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<Node*>> m_adjList;
    std::unordered_map<std::pair<int, int>, Edge*, pair_hash> m_edgeMap;
};

#endif // GRAPH_H
