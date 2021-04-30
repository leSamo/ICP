#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sync_consume.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sync_consume();
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(2);
    AddRoot("hello", "world");
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

    AddChild(itm, "ahoj", "ahojky");
    AddChild(itm, "kamo", "cau");
}

void MainWindow::AddChild(QTreeWidgetItem *parent, QString name, QString desc) {
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, name);
    itm->setText(1, desc);

    parent->addChild(itm);
}
