#include "edge.h"

Edge::Edge(Node* first, Node* second)
    : m_first(first), m_second(second), m_cost(m_cost), m_inMinCut(false)
{
}

Node* Edge::getFirst() const
{
    return m_first;
}

Node* Edge::getSecond() const
{
    return m_second;
}
