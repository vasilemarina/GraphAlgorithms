#include "node.h"

Node::Node(NodeType type) :
    m_type(type)
{
}

int Node::getValue() const
{
    return m_value;
}

QPoint Node::getPos() const
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

NodeType Node::getType() const
{
    return m_type;
}

void Node::setType(NodeType type)
{
    m_type = type;
}
