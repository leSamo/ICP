#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void displayTree();
    void on_blockTree_itemClicked(QTreeWidgetItem *item, int column);
    void on_HelpIt_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
