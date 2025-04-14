#include "graph.h"

Graph::Graph(int nodeRadius)
    : m_nodeRadius(nodeRadius)
{
}

Graph::Graph(const Graph & graph, const std::string& fileName)
    : m_nodes(graph.m_nodes)
    , m_edges(graph.m_edges)
    , m_adjList(graph.m_adjList)
    , m_nodeRadius(graph.m_nodeRadius)
    , m_sortingFileName(fileName)
{

}

Graph::~Graph()
{
    for(Node* n : m_nodes)
        if(n) delete n;
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

    m_adjList.push_back(std::vector<int>(0));
}

void Graph::addEdge(Node* first, Node* second)
{
    if(first == second) return;
    for(int node : m_adjList[first->getValue()-1])
        if(node == second->getValue())
            return;

    Edge e(first, second);
    m_edges.push_back(e);

    m_adjList[first->getValue()-1].push_back(second->getValue());
}

std::vector<Node*> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

void Graph::DFS(Node* startNode)
{
    std::unordered_set<int> unvisited;
    std::stack<int> visited, analyzed;

    visited.push(startNode->getValue());
    for(Node* node : m_nodes)
        if(node != startNode)
            unvisited.insert(node->getValue());

    int t = 1;
    std::vector<int> t1(m_nodes.size(), INT_MAX), t2(m_nodes.size(), INT_MAX);
    t1[startNode->getValue()] = 1;

    while(visited.size())
    {
        int crrNode = visited.top();
        bool analyzedNode = true;
        for(int nextNode : m_adjList[crrNode-1])
            if(unvisited.count(nextNode))
            {
                unvisited.erase(nextNode);
                visited.push(nextNode);
                t1[nextNode-1] = t++;
                analyzedNode = false;
                break;
            }
        if(analyzedNode){
            visited.pop();
            analyzed.push(crrNode);
            t2[crrNode-1] = t++;
        }

    }
}

bool Graph::hasCircuit()
{
    if(m_nodes.size() == 0)
        return false;

    std::unordered_set<int> unvisited;
    std::stack<int> visited, analyzed;

    Node* startNode = m_nodes[0];
    visited.push(startNode->getValue());
    for(Node* node : m_nodes)
        if(node != startNode)
            unvisited.insert(node->getValue());

    int t = 1;
    std::vector<int> t1(m_nodes.size(), INT_MAX), t2(m_nodes.size(), INT_MAX);
    t1[startNode->getValue()-1] = 1;

    bool* inStack = new bool[m_nodes.size()]{false};
    inStack[startNode->getValue()-1] = true;

    while(analyzed.size() < m_nodes.size()){
    while(visited.size())
        {
            int crrNode = visited.top();
            bool analyzedNode = true;
            for(int nextNode : m_adjList[crrNode-1]){
                if(inStack[nextNode-1])
                {
                    delete[] inStack;
                    return true;
                }
                if(unvisited.count(nextNode))
                {
                    unvisited.erase(nextNode);
                    visited.push(nextNode);
                    inStack[nextNode-1] = true;
                    t1[nextNode-1] = t++;
                    analyzedNode = false;
                    break;
                }
            }

            if(analyzedNode){
                inStack[visited.top()-1] = false;
                visited.pop();
                analyzed.push(crrNode);
                t2[crrNode-1] = t++;
            }

        }
        if(unvisited.size())
        for(Node* node : m_nodes)
            if(unvisited.count(node->getValue()))
            {
                startNode = node;
                visited.push(startNode->getValue());
                unvisited.erase(node->getValue());
                t1[node->getValue()-1] = t++;
                break;
            }
    }

    delete[] inStack;
    return false;
}

void Graph::topologicalSort(const std::string &fileName)
{
    if(m_nodes.size() == 0)
        return;

    std::unordered_set<int> unvisited;
    std::stack<int> visited, analyzed;

    Node* startNode = m_nodes[0];
    visited.push(startNode->getValue());
    for(Node* node : m_nodes)
        if(node != startNode)
            unvisited.insert(node->getValue());

    int t = 1;
    std::vector<int> t1(m_nodes.size(), INT_MAX), t2(m_nodes.size(), INT_MAX);
    t1[startNode->getValue()-1] = 1;

    while(analyzed.size() < m_nodes.size()){
        while(visited.size())
        {
            int crrNode = visited.top();
            bool analyzedNode = true;
            for(int nextNode : m_adjList[crrNode-1]){
                if(unvisited.count(nextNode))
                {
                    unvisited.erase(nextNode);
                    visited.push(nextNode);
                    t1[nextNode-1] = t++;
                    analyzedNode = false;

                    for(Edge& edge : m_edges)
                        if(edge.getFirst()->getValue() == crrNode && edge.getSecond()->getValue() == nextNode)
                            edge.m_inTopologicalSorting = true;

                    break;
                }
            }

            if(analyzedNode){
                visited.pop();
                analyzed.push(crrNode);
                t2[crrNode-1] = t++;
            }

        }
        if(unvisited.size())
            for(Node* node : m_nodes)
                if(unvisited.count(node->getValue())){
                    startNode = node;
                    visited.push(startNode->getValue());
                    unvisited.erase(node->getValue());
                    t1[node->getValue()-1] = t++;
                    break;
                }
    }

    std::ofstream file(fileName);
    while(analyzed.size())
    {
        file<< analyzed.top()<<' ';
        analyzed.pop();
    }
    file.close();
}

int Graph::getNodeRadius()
{
    return m_nodeRadius;
}

