#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(rectWidth/2)
    , m_showPath(false)
{
    ui->setupUi(this);

    m_pathButton = new QPushButton("Show paths", this);
    m_pathButton->setGeometry(QRect(QPoint(20, this->size().height()-50), QSize(150, 30)));
    connect(m_pathButton, &QPushButton::released, this, &MainWindow::handleButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    paintGraph(p);
    paintMaze(p, m_graph.getMaze());
    m_pathButton->setGeometry(QRect(QPoint(20, this->size().height()-50), QSize(150, 30)));
}

void MainWindow::paintGraph(QPainter &p)
{
    std::vector<Edge>& edges= m_graph.getEdges();
    for(auto& e : edges)
    {
        p.drawLine(e.getFirst()->getPos().x(), e.getFirst()->getPos().y(),
                   e.getSecond()->getPos().x(), e.getSecond()->getPos().y());
    }

    std::vector<Node*>& nodes = m_graph.getNodes();
    for(Node* n : nodes)
    {
        QRect r(n->getPos().x() - rectWidth/2, n->getPos().y()-rectHeight/2, rectWidth, rectHeight);
        switch(n->getType())
        {
        case NodeType::END:
            p.setBrush(Qt::red);
            break;
        case NodeType::START:
            p.setBrush(Qt::blue);
            break;
        case NodeType::PATH:
            if(m_showPath) p.setBrush(Qt::darkGreen);
            else p.setBrush(Qt::white);
            break;
        default:
            p.setBrush(Qt::white);
            break;
        }

        p.drawEllipse(r);
        QString str;
        str = QString::number(n->getValue());
        QPen pen;
        pen.setColor(Qt::black);
        p.setPen(pen);
        p.drawText(r, Qt::AlignCenter, str);
    }
}

void MainWindow::paintMaze(QPainter& p, std::vector<std::vector<Node *>> maze)
{
    QPen pen;

    for(int i = 0; i < maze.size(); i++)
        for(int j = 0; j < maze.size(); j++){
            if(maze[i][j])
            {
                switch(maze[i][j]->getType()){
                case NodeType::START:
                    p.setBrush(Qt::blue);
                    break;
                case NodeType::END:
                    p.setBrush(Qt::red);
                    break;
                case NodeType::FREE:
                    p.setBrush(Qt::white);
                    break;
                case NodeType::PATH:
                    if(m_showPath) p.setBrush(Qt::darkGreen);
                    else p.setBrush(Qt::white);
                    break;
                }
            }
            else
                p.setBrush(Qt::black);

            pen.setColor(Qt::darkCyan);
            p.setPen(pen);
            p.drawRect(j * CELL_WIDTH + this->size().width()/2+(this->size().width()/2-maze.size()*CELL_WIDTH)/2,
                       i * CELL_HEIGHT + this->size().height()/2-CELL_HEIGHT*(maze.size()/2), CELL_WIDTH, CELL_HEIGHT);
        }
}

void MainWindow::handleButton()
{   
    std::vector<Node*> prevNode;
    std::vector<Node*> exits = m_graph.BFS(m_graph.getStartBFS(), prevNode);
    for(Node* exit : exits)
        m_graph.path(exit, prevNode);

    m_showPath = true;
    update();
}
