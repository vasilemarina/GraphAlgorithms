#include "graph.h"

Graph::Graph(int nodeRadius)
    : m_nodeRadius(nodeRadius)
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

void Graph::addEdge(Node* first, Node* second, QWidget* parent)
{
    if(first == second) return;
    for(Node* node : m_adjList[first->getValue()-1])
        if(node == second)
            return;

    Edge e(first, second);
    bool ok;
    e.m_cost = QInputDialog::getInt(parent, "Edge cost input", "Enter the edge cost:", 0, -1000, 1000, 1, &ok);
    m_edges.push_back(e);

    m_adjList[first->getValue()-1].push_back(second);
    m_edgeMap[{first->getValue(), second->getValue()}] = &m_edges[m_edges.size()-1];
}

void Graph::addEdge(Node *first, Node *second, int cost)
{
    if(first == second) return;
    for(Node* node : m_adjList[first->getValue()-1])
        if(node == second)
            return;

    Edge e(first, second);
    e.m_cost = cost;
    m_edges.push_back(e);

    m_adjList[first->getValue()-1].push_back(second);
    m_edgeMap[{first->getValue(), second->getValue()}] = &m_edges[m_edges.size()-1];
}

bool Graph::dfs(Node *start, Node *end, std::vector<bool> &visited, std::vector<Node*> &parent) const
{
    std::stack<Node*> stack;
    stack.push(start);
    visited[start->getValue()-1] = true;

    while(!stack.empty())
    {
        Node* crrNode = stack.top();
        stack.pop();

        if(crrNode == end)
            return true;

        const std::vector<Node*>& neighbors = m_adjList[crrNode->getValue()-1];
        for(Node* n : neighbors)
            if(!visited[n->getValue()-1] && m_edgeMap.at({crrNode->getValue(), n->getValue()})->m_cost > 0)
            {
                stack.push(n);
                visited[n->getValue()-1] = true;
                parent[n->getValue()-1] = crrNode;
            }
    }

    return false;
}

int Graph::getMaxFlow(Node* start, Node* end, Graph& residualGraph)
{
    int maxFlow = 0;
    getResidualGraph(residualGraph);

    std::vector<Node*> parents(m_nodes.size());
    std::vector<bool> visited(m_nodes.size(), false);

    while(residualGraph.dfs(start, end, visited, parents))
    {
        int pathFlow = INT_MAX;
        for(Node* crrNode = end; crrNode != start; crrNode = parents[crrNode->getValue()-1])
            pathFlow = std::min(pathFlow, residualGraph.m_edgeMap.at({parents[crrNode->getValue()-1]->getValue(), crrNode->getValue()})->m_cost);

        for(Node* crrNode = end; crrNode != start; crrNode = parents[crrNode->getValue()-1])
        {
            residualGraph.m_edgeMap[{parents[crrNode->getValue()-1]->getValue(), crrNode->getValue()}]->m_cost -= pathFlow;
            residualGraph.m_edgeMap[{crrNode->getValue(), parents[crrNode->getValue()-1]->getValue()}]->m_cost += pathFlow;
        }

        maxFlow += pathFlow;
        std::fill(visited.begin(), visited.end(), false);
    }

    findMinCut(residualGraph, start);
    return maxFlow;
}

void Graph::makeEdgeMap()
{
    for(Edge& edge : m_edges)
        m_edgeMap[{edge.getFirst()->getValue(), edge.getSecond()->getValue()}] = &edge;
}

void Graph::getResidualGraph(Graph& residualGraph)
{
    for(Node* n : m_nodes)
        residualGraph.addNode(n->getPos());

    for(const Edge& edge : m_edges)
    {
        residualGraph.addEdge(edge.getFirst(), edge.getSecond(), edge.m_cost);

        if(residualGraph.m_edgeMap.find({edge.getSecond()->getValue(), edge.getFirst()->getValue()}) == residualGraph.m_edgeMap.end())
            residualGraph.addEdge(edge.getSecond(), edge.getFirst());
    }
    residualGraph.makeEdgeMap();
}

void Graph::findMinCut(Graph &residualGraph, Node* startNode) const
{
    std::unordered_set<Node*> unvisited(m_nodes.begin(), m_nodes.end());
    std::vector<Node*> visited(m_nodes.size()), analyzed;
    int head = 0, tail = 0;

    unvisited.erase(startNode);
    visited[tail++] = startNode;

    while(head < tail)
    {
        Node* crrNode = visited[head++];
        for(Node* nextNode : residualGraph.m_adjList[crrNode->getValue()-1])
            if(unvisited.count(nextNode) && residualGraph.m_edgeMap.at({crrNode->getValue(), nextNode->getValue()})->m_cost)
            {
                unvisited.erase(nextNode);
                visited[tail++] = nextNode;
            }
        analyzed.push_back(crrNode);
    }

    for(const Edge& e : residualGraph.m_edges)
        if(unvisited.count(e.getFirst()) && !unvisited.count(e.getSecond())
            || !unvisited.count(e.getFirst()) && unvisited.count(e.getSecond()))
                residualGraph.m_edgeMap.at({e.getFirst()->getValue(), e.getSecond()->getValue()})->m_inMinCut = true;
}

std::vector<Node*> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

std::size_t Graph::pair_hash::operator()(const std::pair<int, int> &p) const
{
    auto h1 = std::hash<int>{}(p.first);
    auto h2 = std::hash<int>{}(p.second);
    return (h1 << 1) ^ h2;
}
