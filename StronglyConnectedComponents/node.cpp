#include "node.h"

Node::Node()
{
}

Node::Node(const Node &n)
{
    nodes.resize(n.nodes.size());
    for(int i = 0; i < n.nodes.size(); i++)
        nodes[i] = n.nodes[i];
}

Node::Node(const std::vector<Node*> & nodes)
    : m_pos(nodes[0]->getPos())
{
    for(Node* node : nodes)
        this->nodes.push_back(node);
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
