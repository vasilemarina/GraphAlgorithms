#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(rectWidth/2)
    , m_showComponents(false)
{
    ui->setupUi(this);
    m_isFirstNodeSelected = false;
    m_dragedNode = nullptr;

    m_showCmpsButton = new QPushButton("Show components", this);
    connect(m_showCmpsButton, &QPushButton::released, this, &MainWindow::handleButton);
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
                        m_graph.addEdge(m_startNode, n);;

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
    if(m_showComponents)
    {
        paintCTC(ev, *(m_graph.m_ctcGraph));
        return;
    }

    m_showCmpsButton->setGeometry(QRect(QPoint(this->size().width()-170, this->size().height()-70), QSize(150, 50)));

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

void MainWindow::paintCTC(QPaintEvent *ev, Graph& CTC)
{
    QPainter p(this);
    std::vector<Node*>& nodes = CTC.getNodes();
    for(Node* n : nodes)
    {
        QRect r(n->getPos().x() - rectWidth/2, n->getPos().y()-rectHeight/2, rectWidth, rectHeight);
        p.drawEllipse(r);
        QString str = QString::number(n->nodes[n->nodes.size()-1]->getValue());
        for(int i = n->nodes.size()-2; i >= 0; i--){
            str.append(", ");
            str.append(QString::number(n->nodes[i]->getValue()));
        }
        QPen pen;
        pen.setColor(Qt::red);
        p.setPen(pen);
        p.drawText(r, Qt::AlignCenter, str);
        pen.setColor(Qt::black);
        p.setPen(pen);
    }

    std::vector<Edge>& edges = CTC.getEdges();
    for(auto& e : edges)
    {
        p.drawLine(e.getFirst()->getPos().x(), e.getFirst()->getPos().y(),
                   e.getSecond()->getPos().x(), e.getSecond()->getPos().y());

        double angle = atan2((-e.getSecond()->getPos().y()+e.getFirst()->getPos().y()),
                             (e.getSecond()->getPos().x() - e.getFirst()->getPos().x()));

        double arrowSize = CTC.getNodeRadius() - CTC.getNodeRadius() / 4 ;

        QPoint p1(e.getSecond()->getPos().x(), e.getSecond()->getPos().y());
        QPoint p2(p1.x() - arrowSize * std::cos(angle-M_PI_4), -(-p1.y() - arrowSize * std::sin(angle-M_PI_4)));
        QPoint p3(p1.x() - arrowSize * std::cos(angle+M_PI_4), -(-p1.y() - arrowSize * std::sin(angle+M_PI_4)));

        p.setBrush(Qt::darkGray);
        p.drawPolygon(QPolygon({p1, p2, p3}));
    }
}

void MainWindow::handleButton()
{
    m_showComponents = true;
    m_graph.CTC();

    Graph* ctcGraph = new Graph(rectWidth/2);
    for(std::vector<Node*>& c : m_graph.m_components){
        Node cmp(c);
        ctcGraph->addNode(cmp);
    }

    std::vector<Node*>& nodes = ctcGraph->getNodes();
    for(Node* i : nodes)
        for(Node* j : nodes)
            if(i != j && m_graph.hasEdge(i, j))
                ctcGraph->addEdge(i, j);

    m_graph.m_ctcGraph = ctcGraph;

    delete m_showCmpsButton;
    update();
}

