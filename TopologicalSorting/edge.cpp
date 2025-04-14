#include "edge.h"

Edge::Edge()
{
}

Edge::Edge(Node* first, Node* second)
    : m_first(first), m_second(second), m_inTopologicalSorting(false)
{
}

Node* Edge::getFirst()
{
    return m_first;
}

Node* Edge::getSecond()
{
    return m_second;
}

