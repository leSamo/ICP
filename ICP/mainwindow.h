#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mqtt/properties.h"
#include "mqtt/async_client.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

void AddRoot(QString name, QString desc);
void AddChild(QTreeWidgetItem *parent, QString name, QString desc);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
