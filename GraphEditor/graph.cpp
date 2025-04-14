#include "graph.h"

Graph::Graph(bool isOriented, int nodeRadius, QString fileName)
    : m_isOriented(isOriented)
     ,m_nodeRadius(nodeRadius)
     ,m_adjMatrixFileName(fileName)
{
}

Graph::~Graph()
{
    for(Node* n : m_nodes)
        delete n;
}

void Graph::addNode(QPoint p)
{
    for(Node* n : m_nodes)
        if((n->getPos().x() - p.x())*(n->getPos().x() - p.x()) +
            (n->getPos().y() - p.y())*(n->getPos().y() - p.y()) < 4*m_nodeRadius*m_nodeRadius)
            return;

    Node* n = new Node;
    n->setPosition(p);
    n->setValue(m_nodes.size()+1);
    m_nodes.push_back(n);

    for(std::vector<int>& row : m_adjMatrix)
        row.push_back(0);
    std::vector<int> newRow(m_adjMatrix.size() + 1, 0);
    m_adjMatrix.push_back(newRow);

    m_adjList.push_back(std::vector<int>(0));
}

void Graph::addEdge(Node* first, Node* second)
{
    if(first == second) return;
    for(int node : m_adjList[first->getValue()-1])
        if(node == second->getValue()-1)
            return;

    Edge e(first, second);
    m_edges.push_back(e);

    m_adjMatrix[first->getValue()-1][second->getValue()-1] = 1;
    m_adjList[first->getValue()-1].push_back(second->getValue()-1);
    if(!m_isOriented){
        m_adjMatrix[second->getValue()-1][first->getValue()-1] = 1;
        m_adjList[second->getValue()-1].push_back(first->getValue()-1);
    }
}

std::vector<Node*> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

int Graph::getNodeRadius()
{
    return m_nodeRadius;
}

void Graph::saveMAtoFile(const QString& filename)
{
    QFile file(filename);

    if(file.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&file);

        out<< m_adjMatrix.size()<<'\n';
        for(std::vector<int>& row : m_adjMatrix){
            for(int i : row)
                out<< i<<' ';
            out<< '\n';
        }
    }
}

