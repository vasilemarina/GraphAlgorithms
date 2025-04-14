#ifndef NODE_H
#define NODE_H

#include <QPoint>

enum class NodeType
{
    FREE = 1,
    END = 2,
    START = 3,
    PATH = 4
};

class Node
{
public:
    Node(NodeType type = NodeType::FREE);

    int getValue() const;
    QPoint getPos() const;
    void setValue(int n);
    void setPosition(QPoint p);
    NodeType getType() const;
    void setType(NodeType);
private:
    int m_value;
    QPoint m_pos;
    NodeType m_type;
};

#endif // NODE_H
