#ifndef NODE_H
#define NODE_H

#include <QPoint>
class Node
{
public:
    Node();
    int getValue();
    QPoint getPos();
    void setValue(int n);
    void setPosition(QPoint p);
private:
    int m_value;
    QPoint m_pos;
};

#endif // NODE_H
