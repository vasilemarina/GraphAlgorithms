#include "graph.h"

Graph::Graph(int nodeRadius)
    : m_nodeRadius(nodeRadius)
    , m_ctcGraph(nullptr)
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

    m_adjList.push_back(std::vector<Node*>(0));
}

void Graph::addNode(Node& n)
{
    addNode(n.getPos());
    std::vector<Node*> & nodes = getNodes();

    for(Node* cmpNode : n.nodes)
        nodes[nodes.size()-1]->nodes.push_back(cmpNode);
}

void Graph::addEdge(Node* first, Node* second)
{
    if(first == second) return;
    for(Node* node : m_adjList[first->getValue()-1])
        if(node == second)
            return;

    Edge e(first, second);
    m_edges.push_back(e);

    m_adjList[first->getValue()-1].push_back(second);
}

std::vector<Node*> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

std::vector<Node*> Graph::DFS(Node* startNode, std::unordered_set<Node*>& unvisited)
{
    std::stack<Node*> visited;
    std::vector<Node*> analyzed;

    visited.push(startNode);

    unvisited.erase(startNode);
    while(visited.size())
    {
        Node* crrNode = visited.top();
        bool analyzedNode = true;
        for(Node* nextNode : m_adjList[crrNode->getValue()-1])
            if(unvisited.count(nextNode))
            {
                unvisited.erase(nextNode);
                visited.push(nextNode);
                analyzedNode = false;
                break;
            }
        if(analyzedNode){
            visited.pop();
            analyzed.push_back(crrNode);
        }
    }
    return analyzed;
}

std::stack<Node*> Graph::TDFS()
{
    if(m_nodes.size() == 0)
        return std::stack<Node*>();

    std::unordered_set<Node*> unvisited;
    std::stack<Node*> visited, analyzed;

    Node* startNode = m_nodes[0];
    visited.push(startNode);
    for(Node* node : m_nodes)
        if(node != startNode)
            unvisited.insert(node);

    while(analyzed.size() < m_nodes.size()){
        while(visited.size())
        {
            Node* crrNode = visited.top();
            bool analyzedNode = true;
            for(Node* nextNode : m_adjList[crrNode->getValue()-1])
                if(unvisited.count(nextNode))
                {
                    unvisited.erase(nextNode);
                    visited.push(nextNode);
                    analyzedNode = false;
                    break;
                }

            if(analyzedNode){
                visited.pop();
                analyzed.push(crrNode);
            }
        }
        if(unvisited.size())
            for(Node* node : m_nodes)
                if(unvisited.count(node))
                {
                    startNode = node;
                    visited.push(startNode);
                    unvisited.erase(node);
                }
    }

    return analyzed;
}

void Graph::findComponents(std::stack<Node*> dfs, std::vector<std::vector<Node*>>& components)
{
    if(m_nodes.size() == 0)
        return;

    std::unordered_set<Node*> unvisited;
    std::stack<Node*> visited, analyzed;

    for(Node* node : m_nodes)
        unvisited.insert(node);

    while(!dfs.empty())
    {
        Node* startNode = dfs.top();
        dfs.pop();

        if(unvisited.count(startNode))
        {
            std::vector<Node*> cmp = DFS(startNode, unvisited);
            components.push_back(cmp);

            for(Node* node : cmp)
                unvisited.erase(node);
        }
    }
}

void Graph::CTC()
{
    std::stack<Node*> t2 = TDFS();
    reverse();
    findComponents(t2, m_components);

    reverse();
}

void Graph::reverse()
{
    m_adjList.clear();
    m_adjList.resize(m_nodes.size());
    for(Edge& edge : m_edges){
        edge.swapNodes();
        m_adjList[edge.getFirst()->getValue()-1].push_back(edge.getSecond());
    }
}

bool Graph::hasEdge(Node* i, Node* j)
{
    for(Node* cmpI: i->nodes)
        for(Node* cmpJ : j->nodes)
            for(Edge& e : m_edges)
                if(e.getFirst() == cmpI && e.getSecond() == cmpJ)
                    return true;

    return false;
}

int Graph::getNodeRadius()
{
    return m_nodeRadius;
}

