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
    ~Graph();
    void addNode(QPoint p);
    void addNode(Node&);
    void addEdge(Node* first, Node* second);
    std::vector<Node*> DFS(Node* startNode, std::unordered_set<Node*>& unvisited);
    std::stack<Node*> TDFS();
    void findComponents(std::stack<Node*> dfs, std::vector<std::vector<Node*>>& cmp);
    void CTC();
    void reverse();
    bool hasEdge(Node* i, Node* j);

    int getNodeRadius();
    std::vector<Node*>& getNodes();
    std::vector<Edge>& getEdges();

    std::vector<std::vector<Node*>> m_components;
    Graph* m_ctcGraph;
private:
    int m_nodeRadius;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<Node*>> m_adjList;
};

#endif // GRAPH_H
