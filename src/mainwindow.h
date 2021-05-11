/*! \file mainwindow.h
 * ICP project 2020/21
 * Authors: Michal Findra, Samuel Olekšák
 */


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
#include "publisher.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnAdd_item_clicked();
    void on_actionHelp_triggered();
    void onRemove();
    void onSend();

private:
    Ui::MainWindow *ui;
    HelpDialog *help;
    std::deque<msg> msgs;
    std::string serverAddress;

    QTreeWidgetItem* AttachOrRefreshNode(QTreeWidgetItem *parent, QString topicSegment, QString msgContent, QString timeOfMsgReceived, QString topic);
    void AddToDash(QString desc, QString topic);


public slots:
    void DisplayMsg(QString topic, QString msg);
    void showTopicHistory(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
