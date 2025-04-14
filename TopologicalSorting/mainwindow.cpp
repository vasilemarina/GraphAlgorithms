#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(rectWidth/2)
    ,m_showTopologicalSorting(false)
{
    ui->setupUi(this);
    m_dragedNode = nullptr;
    m_isFirstNodeSelected = false;

    m_topologicalSortingButton = new QPushButton("Topological Sorting", this);
    m_topologicalSortingButton->setGeometry(QRect(QPoint(this->size().width()-170, this->size().height()-70), QSize(150, 50)));
    connect(m_topologicalSortingButton, &QPushButton::released, this, &MainWindow::handleButton);
}

MainWindow::MainWindow(const Graph &graph)
    : ui(new Ui::MainWindow)
    , m_graph(graph, "topologicalSorting.out")
    ,m_showTopologicalSorting(true)
{
    ui->setupUi(this);
    setWindowTitle("Topological sorting");

    m_dragedNode = nullptr;
    m_isFirstNodeSelected = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* m)
{
    if(!m_showTopologicalSorting && m->button() == Qt::RightButton)
    {
        m_graph.addNode(m->pos());
        update();
    }
    else if(m->button() == Qt::LeftButton)
    {
        if((m_pressMousePos.x()-m->pos().x())*(m_pressMousePos.x()-m->pos().x()) +
                (m_pressMousePos.y()-m->pos().y())*(m_pressMousePos.y()-m->pos().y()) > QApplication::startDragDistance())
        {
            std::vector<Node*>& nodes = m_graph.getNodes();
            for(Node* n : nodes)
                if(n != m_dragedNode && (n->getPos().x() - m_dragedNode->getPos().x())*(n->getPos().x() -  m_dragedNode->getPos().x()) +
                        (n->getPos().y() -  m_dragedNode->getPos().y())*(n->getPos().y() - m_dragedNode->getPos().y()) < rectWidth*rectWidth)
                {
                    m_dragedNode->setPosition(m_pressMousePos);
                    update();
                    break;
                }
            m_dragedNode = nullptr;
        }
        else if(!m_showTopologicalSorting){
            std::vector<Node*>& nodes = m_graph.getNodes();
            for(Node* n : nodes)
            {
                if((n->getPos().x() - m->pos().x()) *(n->getPos().x() - m->pos().x()) +
                        (n->getPos().y() - m->pos().y()) *(n->getPos().y() - m->pos().y()) < rectWidth * rectWidth/4)
                {
                    if(m_isFirstNodeSelected)
                    {
                        m_graph.addEdge(m_startNode, n);

                        m_isFirstNodeSelected = false;
                        update();
                    }
                    else
                    {
                        m_startNode = n;
                        m_isFirstNodeSelected = true;
                    }
                }
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    m_pressMousePos = m->pos();
    std::vector<Node*>& nodes = m_graph.getNodes();
    for(Node* n : nodes)
        if((n->getPos().x() - m->pos().x()) *(n->getPos().x() - m->pos().x()) +
                (n->getPos().y() - m->pos().y()) *(n->getPos().y() - m->pos().y()) < rectWidth * rectWidth/4)
        {
            m_dragedNode = n;
            break;
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    if(m_dragedNode && (m_pressMousePos.x()-m->pos().x())*(m_pressMousePos.x()-m->pos().x()) +
            (m_pressMousePos.y()-m->pos().y())*(m_pressMousePos.y()-m->pos().y()) > QApplication::startDragDistance())
    {
        m_dragedNode->setPosition(m->pos());
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent* ev)
{
    if(!m_showTopologicalSorting)
        m_topologicalSortingButton->setGeometry(QRect(QPoint(this->size().width()-170, this->size().height()-70), QSize(150, 50)));

    QPainter p(this);
    std::vector<Node*>& nodes = m_graph.getNodes();
    for(Node* n : nodes)
    {
        QRect r(n->getPos().x() - rectWidth/2, n->getPos().y()-rectHeight/2, rectWidth, rectHeight);
        p.drawEllipse(r);
        QString str;
        str = QString::number(n->getValue());
        QPen pen;
        pen.setColor(Qt::red);
        p.setPen(pen);
        p.drawText(r, Qt::AlignCenter, str);
        pen.setColor(Qt::black);
        p.setPen(pen);
    }

    std::vector<Edge>& edges = m_graph.getEdges();
    for(auto& e : edges)
        if(m_showTopologicalSorting == e.m_inTopologicalSorting)
        {
            p.drawLine(e.getFirst()->getPos().x(), e.getFirst()->getPos().y(),
                       e.getSecond()->getPos().x(), e.getSecond()->getPos().y());

            double angle = atan2((-e.getSecond()->getPos().y()+e.getFirst()->getPos().y()),
                                 (e.getSecond()->getPos().x() - e.getFirst()->getPos().x()));

            double arrowSize = m_graph.getNodeRadius() - m_graph.getNodeRadius() / 4 ;

            QPoint p1(e.getSecond()->getPos().x(), e.getSecond()->getPos().y());
            QPoint p2(p1.x() - arrowSize * std::cos(angle-M_PI_4), -(-p1.y() - arrowSize * std::sin(angle-M_PI_4)));
            QPoint p3(p1.x() - arrowSize * std::cos(angle+M_PI_4), -(-p1.y() - arrowSize * std::sin(angle+M_PI_4)));

            p.setBrush(Qt::darkGray);
            p.drawPolygon(QPolygon({p1, p2, p3}));
        }
}

void MainWindow::handleButton()
{
    if(m_graph.hasCircuit())
    {
        std::cout<< "The graph has circuits. Topological sorting can't be done.\n";
        close();
    }
    else
    {
        MainWindow* newWindow = new MainWindow(m_graph);

        newWindow->m_graph.topologicalSort(newWindow->m_graph.m_sortingFileName);
        newWindow->show();
    }
}

