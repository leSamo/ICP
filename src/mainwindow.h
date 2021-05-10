#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mqtt/properties.h"
#include "mqtt/async_client.h"
#include "callback.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTreeWidget>
#include "helpdialog.h"
#include <deque>
#include "message.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    QTreeWidgetItem* AddRoot(QString name, QString desc, QString time, QString path);
    QTreeWidgetItem* AddChild(QTreeWidgetItem *parent, QString name, QString desc, QString time, QString path);
    void AddToDash(QString desc, QString topic);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnAdd_item_clicked();
    void on_actionHelp_triggered();
    void onRemove();

private:
    Ui::MainWindow *ui;
    HelpDialog *help;
    std::deque<msg> msgs;


public slots:
    void DisplayMsg(QString topic, QString msg);
    void on_treewidget_clicked(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
