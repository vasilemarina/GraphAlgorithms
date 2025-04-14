#include "graph.h"

Graph::Graph(int nodeRadius)
    : m_nodeRadius(nodeRadius)
{
    std::vector<std::vector<int>> mazeMatrix;
    readMatrix("mazeMatrix.in", mazeMatrix);

    m_nodesMatrix.resize(mazeMatrix.size());
    for(int i = 0; i < mazeMatrix.size(); i++){
        m_nodesMatrix[i].resize(mazeMatrix[0].size());
        for(int j = 0; j < mazeMatrix[0].size(); j++)
            if(mazeMatrix[i][j])
            {
                m_nodesMatrix[i][j] = addNode(QPoint((j+1)*50, (i+1)*50), (NodeType)mazeMatrix[i][j]);

                if(mazeMatrix[i][j] == (int)NodeType::START)
                    m_startBFS = m_nodesMatrix[i][j];
            }
    }

    for(int i = 0; i < m_nodesMatrix.size(); i++)
        for(int j = 0; j < m_nodesMatrix[0].size(); j++)
        {
            if(i-1 >= 0 && m_nodesMatrix[i][j] && m_nodesMatrix[i-1][j])
                addEdge(m_nodesMatrix[i][j], m_nodesMatrix[i-1][j]);
            if(j-1 >= 0 && m_nodesMatrix[i][j] && m_nodesMatrix[i][j-1])
                addEdge(m_nodesMatrix[i][j], m_nodesMatrix[i][j-1]);
        }
}

Graph::~Graph()
{
    for(Node* n : m_nodes)
        delete n;
}

Node* Graph::addNode(QPoint p, NodeType type)
{
    for(Node* n : m_nodes)
        if((n->getPos().x() - p.x())*(n->getPos().x() - p.x()) +
            (n->getPos().y() - p.y())*(n->getPos().y() - p.y()) < 4*m_nodeRadius*m_nodeRadius)
            return nullptr;

    Node* n = new Node(type);
    n->setPosition(p);
    n->setValue(m_nodes.size()+1);
    m_nodes.push_back(n);

    m_adjList.push_back(std::vector<Node*>(0));

    return n;
}

void Graph::addEdge(Node* first, Node* second)
{
    if(first == second) return;

    Edge e(first, second);
    m_edges.push_back(e);

    m_adjList[first->getValue()-1].push_back(second);
    m_adjList[second->getValue()-1].push_back(first);
}

std::vector<Node*> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

const std::vector<std::vector<Node *> > &Graph::getMaze() const
{
    return m_nodesMatrix;
}

Node *Graph::getStartBFS() const
{
    return m_startBFS;
}

std::vector<Node *> Graph::BFS(Node* startNode, std::vector<Node*>& predecessor)
{
    std::unordered_set<Node*> unvisited(m_nodes.begin(), m_nodes.end());
    std::vector<Node*> visited(m_nodes.size()), analyzed;
    int head = 0, tail = 0;
    std::vector<int> length(m_nodes.size());
    std::vector<Node*> exits;

    predecessor.resize(m_nodes.size());
    unvisited.erase(startNode);
    visited[tail++] = startNode;

    predecessor[startNode->getValue()-1] = nullptr;
    for(Node* node : m_nodes) predecessor[node->getValue()-1] = nullptr;

    length[startNode->getValue()-1] = 0;
    for(Node* node : unvisited) length[node->getValue()-1] = INT_MAX;

    while(head < tail)
    {
        Node* crrNode = visited[head++];
        if(crrNode->getType() == NodeType::END)
            exits.push_back(crrNode);
        for(Node* nextNode : m_adjList[crrNode->getValue()-1])
            if(unvisited.count(nextNode))
            {
                unvisited.erase(nextNode);
                visited[tail++] = nextNode;
                predecessor[nextNode->getValue()-1] = crrNode;
                length[nextNode->getValue()-1] = length[crrNode->getValue()-1] + 1;
            }
        analyzed.push_back(crrNode);
    }

    return exits;
}

void Graph::path(Node *endNode, const std::vector<Node*>& predecessor) const
{
    while(predecessor[endNode->getValue()-1] != nullptr)
    {
        endNode = predecessor[endNode->getValue()-1];
        if(endNode->getType() == NodeType::FREE)
            endNode->setType(NodeType::PATH);
    }
}

void Graph::readMatrix(const std::string& fileName, std::vector<std::vector<int>>& matrix)
{
    std::ifstream file(fileName);
    if(!file.is_open())
        std::cout<< "Error: The file "<< fileName<< " couldn't be opened.\n";

    std::string fileRow;
    while(file>> fileRow)
    {
        std::vector<int> row;
        row.resize(fileRow.size());
        for(int i = 0; i < row.size(); i++)
            row[i] = fileRow[i] - '0';

        matrix.push_back(row);
    }
}


