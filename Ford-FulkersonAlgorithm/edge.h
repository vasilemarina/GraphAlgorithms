#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include <QLineEdit>

class Edge
{
public:
    Edge(Node* first, Node* second);
    Node* getFirst() const;
    Node* getSecond() const;

    int m_cost;
    bool m_inPath;
    bool m_inMinCut;
private:
    Node* m_first, *m_second;
};

#endif // EDGE_H
