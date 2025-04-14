#include "edge.h"

Edge::Edge()
{
}

Edge::Edge(Node* first, Node* second)
    : m_first(first), m_second(second)
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

void Edge::swapNodes()
{
    std::swap(m_first, m_second);
}

