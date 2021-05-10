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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    QTreeWidgetItem* AddRoot(QString name, QString desc, QString time, QString path);
    QTreeWidgetItem* AddChild(QTreeWidgetItem *parent, QString name, QString desc, QString time, QString path);

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

public slots:
    void DisplayMsg(QString topic, QString msg);
};

#endif // MAINWINDOW_H
