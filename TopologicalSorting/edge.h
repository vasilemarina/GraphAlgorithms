#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
public:
    Edge();
    Edge(Node* first, Node* second);
    Node* getFirst();
    Node* getSecond();

    bool m_inTopologicalSorting;
private:
    Node* m_first, *m_second;
};

#endif // EDGE_H
