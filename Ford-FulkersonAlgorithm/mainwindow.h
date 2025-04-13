#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <iostream>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mouseReleaseEvent(QMouseEvent* m) override;
    void mousePressEvent(QMouseEvent* m) override;
    void mouseMoveEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void paintEdges();
    void paintNodes();

private slots:
    void handleButton();
private:
    Ui::MainWindow *ui;
    const int rectWidth;
    const int rectHeight;
    Graph m_graph, m_residualGraph;
    Node* m_startNode;
    Node* m_dragedNode;
    QPoint m_pressMousePos;
    bool m_isFirstNodeSelected;
    QPushButton* m_showMinCutButton;
    bool m_showMinCut;
};
#endif // MAINWINDOW_H
