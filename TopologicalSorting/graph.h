#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <stack>
#include <QString>
#include <QFile>
#include <QTextStream>

class Graph
{
public:
    Graph(int nodeRadius);
    Graph(const Graph&, const std::string& fileName);
    ~Graph();
    void addNode(QPoint p);
    void addEdge(Node* first, Node* second);
    void DFS(Node* startNode);
    bool hasCircuit();
    void topologicalSort(const std::string& fileName);

    int getNodeRadius();
    std::vector<Node*>& getNodes();
    std::vector<Edge>& getEdges();

    std::string m_sortingFileName;
private:
    int m_nodeRadius;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<int>> m_adjList;
};

#endif // GRAPH_H
