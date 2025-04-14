#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <vector>
class Node
{
public:
    Node(const std::vector<Node*>&);
    Node();
    Node(const Node& n);
    int getValue();
    QPoint getPos();
    void setValue(int n);
    void setPosition(QPoint p);
    std::vector<Node*> nodes;
private:
    int m_value;
    QPoint m_pos;
};

#endif // NODE_H
