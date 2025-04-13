#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(rectWidth/2)
    , m_residualGraph(rectWidth)
    , m_showMinCut(false)
    , rectWidth(40)
    , rectHeight(40)
{
    ui->setupUi(this);
    m_isFirstNodeSelected = false;
    m_dragedNode = nullptr;

    m_showMinCutButton = new QPushButton("Show Minimum Cut", this);
    connect(m_showMinCutButton, &QPushButton::released, this, &MainWindow::handleButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* m)
{
    if(m->button() == Qt::RightButton)
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
        else{
            std::vector<Node*>& nodes = m_graph.getNodes();
            for(Node* n : nodes)
            {
                if((n->getPos().x() - m->pos().x()) *(n->getPos().x() - m->pos().x()) +
                        (n->getPos().y() - m->pos().y()) *(n->getPos().y() - m->pos().y()) < rectWidth * rectWidth/4)
                {
                    if(m_isFirstNodeSelected)
                    {
                        m_graph.addEdge(m_startNode, n, this);

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
    m_showMinCutButton->setGeometry(QRect(QPoint(this->size().width()-190, this->size().height()-70), QSize(170, 50)));

    paintNodes();
    paintEdges();
}

void MainWindow::paintEdges()
{
    QPainter p(this);
    QFont font = p.font();
    font.setPointSize(10);
    p.setFont(font);

    std::vector<Edge>& edges = m_graph.getEdges();
    if(m_showMinCut)
        edges = m_residualGraph.getEdges();

    for(auto& e : edges)
    if(e.m_cost){
        QPen pen;
        pen.setWidth(3);
        if(m_showMinCut && e.m_inMinCut)
            pen.setColor(Qt::blue);
        p.setPen(pen);
        p.drawLine(e.getFirst()->getPos().x(), e.getFirst()->getPos().y(),
                   e.getSecond()->getPos().x(), e.getSecond()->getPos().y());

        int boxX = e.getFirst()->getPos().x() + (e.getSecond()->getPos().x() - e.getFirst()->getPos().x()) / 2;
        int boxY = e.getFirst()->getPos().y() + (e.getSecond()->getPos().y() - e.getFirst()->getPos().y()) / 2;
        if(e.getFirst()->getPos().x() < e.getSecond()->getPos().x())
        {
            p.setBrush(Qt::black);
            boxY -= rectHeight;
        }
        else if(e.getFirst()->getPos().x() > e.getSecond()->getPos().x())
        {
            pen.setColor(Qt::red);
            p.setPen(pen);
            p.setBrush(Qt::red);
            boxY += rectHeight / 2;
        }
        else if(e.getFirst()->getPos().y() < e.getSecond()->getPos().y())
        {
            p.setBrush(Qt::black);
            boxX -= rectWidth / 2;
        }
        else
        {
            pen.setColor(Qt::red);
            p.setPen(pen);
            p.setBrush(Qt::red);
            boxX += rectWidth / 4;
        }

        QRect costRect(boxX, boxY, rectWidth, rectHeight);
        p.drawText(costRect, QString::number(e.m_cost));

        double angle = atan2((-e.getSecond()->getPos().y() + e.getFirst()->getPos().y()),
                             (e.getSecond()->getPos().x() - e.getFirst()->getPos().x()));
        double arrowSize = rectWidth / 4;
        QPoint p1(e.getSecond()->getPos().x(), e.getSecond()->getPos().y());
        QPoint p2(p1.x() - arrowSize * std::cos(angle-M_PI_4), -(-p1.y() - arrowSize * std::sin(angle-M_PI_4)));
        QPoint p3(p1.x() - arrowSize * std::cos(angle+M_PI_4), -(-p1.y() - arrowSize * std::sin(angle+M_PI_4)));

        pen.setWidth(2);
        pen.setColor(Qt::black);
        p.setPen(pen);
        p.drawPolygon(QPolygon({p1, p2, p3}));
    }
}

void MainWindow::paintNodes()
{
    QPainter p(this);
    QFont font = p.font();
    font.setPointSize(20);
    p.setFont(font);

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
}

void MainWindow::handleButton()
{
    m_showMinCut = true;

    std::vector<Node*>& nodes = m_graph.getNodes();
    std::cout<< "Maximum flow: "<< m_graph.getMaxFlow(nodes[0], nodes[nodes.size()-1], m_residualGraph)<< '\n';

    update();
}

