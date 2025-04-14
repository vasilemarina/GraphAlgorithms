#include "node.h"

Node::Node()
{
}

int Node::getValue()
{
    return m_value;
}

QPoint Node::getPos()
{
    return m_pos;
}

void Node::setValue(int n)
{
    m_value = n;
}

void Node::setPosition(QPoint p)
{
    m_pos = p;
}
