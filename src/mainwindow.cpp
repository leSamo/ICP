#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"
#include <regex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    displayTree();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMap<QString, QString> map;

void MainWindow::displayTree()
{
    ui->blockTree->setColumnCount(1);
    ui->blockTree->setHeaderLabel("Block categories");

    QTreeWidgetItem *logic_root = new QTreeWidgetItem(ui->blockTree);
    logic_root->setText(0, "Logic blocks");
    ui->blockTree->addTopLevelItem(logic_root);

    QTreeWidgetItem *lchild1 = new QTreeWidgetItem();
    lchild1->setText(0, "AND block");
    logic_root->addChild(lchild1);

    QTreeWidgetItem *lchild2 = new QTreeWidgetItem();
    lchild2->setText(0, "OR block");
    logic_root->addChild(lchild2);

    QTreeWidgetItem *lchild3 = new QTreeWidgetItem();
    lchild3->setText(0, "NOT block");
    logic_root->addChild(lchild3);

    QTreeWidgetItem *math_root = new QTreeWidgetItem(ui->blockTree);
    math_root->setText(0, "Aritmetic blocks");
    ui->blockTree->addTopLevelItem(math_root);

    QTreeWidgetItem *mchild1 = new QTreeWidgetItem();
    mchild1->setText(0, "ADD block");
    math_root->addChild(mchild1);

    QTreeWidgetItem *mchild2 = new QTreeWidgetItem();
    mchild2->setText(0, "SUB block");
    math_root->addChild(mchild2);

    QTreeWidgetItem *mchild3 = new QTreeWidgetItem();
    mchild3->setText(0, "MUL block");
    math_root->addChild(mchild3);

    QTreeWidgetItem *mchild4 = new QTreeWidgetItem();
    mchild4->setText(0, "DIV block");
    math_root->addChild(mchild4);

    // arithmetic blocks declaration
    map.insert("ADD block", "int ADD(int a, int b) {\n   return a + b\n}");
    map.insert("SUB block", "int SUB(int a, int b) {\n   return a - b\n}");
    map.insert("MUL block", "int MUL(int a, int b) {\n   return a * b\n}");
    map.insert("DIV block", "int DIV(int a, int b) {\n   return a / b\n}");

    // logic blocks declaration
    map.insert("AND block", "int AND(int a, int b) {\n   return a && b\n}");
    map.insert("OR block", "int OR(int a, int b) {\n   return a || b\n}");
    map.insert("NOT block", "int NOT(int a) {\n   return !a \n}");

}

void MainWindow::on_blockTree_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(0) == "ADD block"){
          ui->CodeEditor->setText(map["ADD block"]);
    } else if (item->text(0) == "SUB block") {
        ui->CodeEditor->setText(map["SUB block"]);
    } else if (item->text(0) == "MUL block") {
        ui->CodeEditor->setText(map["MUL block"]);
    } else if (item->text(0) == "DIV block") {
        ui->CodeEditor->setText(map["DIV block"]);
    } else if (item->text(0) == "AND block") {
        ui->CodeEditor->setText(map["AND block"]);
    } else if (item->text(0) == "OR block") {
        ui->CodeEditor->setText(map["OR block"]);
    } else if (item->text(0) == "NOT block") {
        ui->CodeEditor->setText(map["NOT block"]);
    }
}

void MainWindow::on_HelpIt_triggered()
{
    HelpDialog hd;
    hd.setModal(true);
    hd.exec();
}
