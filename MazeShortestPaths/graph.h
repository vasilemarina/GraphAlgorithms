#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <limits>

class Graph
{
public:
    Graph(int nodeRadius);
    ~Graph();
    Node* addNode(QPoint p, NodeType type = NodeType::FREE);
    void addEdge(Node* first, Node* second);

    std::vector<Node*>& getNodes();
    std::vector<Edge>& getEdges();
    const std::vector<std::vector<Node*>>& getMaze() const;
    Node* getStartBFS() const;

    std::vector<Node*> BFS(Node* startNode, std::vector<Node*>& predecessor);
    void path(Node* endNode, const std::vector<Node*>& predecessor) const;
private:
    static void readMatrix(const std::string& fileName, std::vector<std::vector<int>>& matrix);

    int m_nodeRadius;
    Node* m_startBFS;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<Node*>> m_nodesMatrix;
    std::vector<std::vector<Node*>> m_adjList;
};

#endif // GRAPH_H
