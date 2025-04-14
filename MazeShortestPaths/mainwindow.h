#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void paintEvent(QPaintEvent* ev) override;
    void paintGraph(QPainter& p);
    void paintMaze(QPainter& p, std::vector<std::vector<Node*>> maze);

private slots:
    void handleButton();
private:
    Ui::MainWindow *ui;
    const int rectWidth = 40;
    const int rectHeight = 40;
    const int CELL_WIDTH = 40;
    const int CELL_HEIGHT = 30;

    Graph m_graph;
    QPushButton* m_pathButton;
    bool m_showPath;
};
#endif // MAINWINDOW_H
