#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "async_consume.h"
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddRoot(QString name, QString desc) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0, name);
    itm->setText(1, desc);

    ui->treeWidget->addTopLevelItem(itm);

    //AddChild(itm, "ahoj", "ahojky");
    //AddChild(itm, "kamo", "cau");
}

void MainWindow::AddChild(QTreeWidgetItem *parent, QString name, QString desc) {
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, name);
    itm->setText(1, desc);

    parent->addChild(itm);
}

void MainWindow::on_btnConnect_clicked()
{
    QString server = ui->inputServer->toPlainText();
    QString topic = ui->inputTopic->toPlainText();
    ui->txtTitle->setText(topic);

    AddRoot(topic, "");

    if (isConnected()) {
        disconnect();
    }

    setTopic(topic.toUtf8().constData());

    async_consume(server.toUtf8().constData());
}
