#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPushButton>
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
    void paintCTC(QPaintEvent* ev, Graph& ctc);

    bool m_showComponents;

private slots:
    void handleButton();
private:
    Ui::MainWindow *ui;
    const int rectWidth = 40;
    const int rectHeight = 40;

    Graph m_graph;
    Node* m_startNode;
    Node* m_dragedNode;
    QPoint m_pressMousePos;
    bool m_isFirstNodeSelected;
    QPushButton* m_showCmpsButton;
};
#endif // MAINWINDOW_H
