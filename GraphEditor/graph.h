#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <QString>
#include <QFile>
#include <QTextStream>

class Graph
{
public:
    Graph(bool isOriented, int nodeRadius, QString fileName);
    ~Graph();
    void addNode(QPoint p);
    void addEdge(Node* first, Node* second);
    void saveMAtoFile(const QString& filename);

    int getNodeRadius();
    std::vector<Node*>& getNodes();
    std::vector<Edge>& getEdges();
    bool m_isOriented;
    QString m_adjMatrixFileName;
private:
    int m_nodeRadius;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<int>> m_adjMatrix;
    std::vector<std::vector<int>> m_adjList;
};

#endif // GRAPH_H
