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


void MainWindow::on_HelpButton_clicked()
{
    HelpDialog hd;
    hd.setModal(true);
    hd.exec();
}

QVector<QPair<QString, QString>> *blockVector = new QVector<QPair<QString, QString>>;


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


    QPair<QString, QString> math_add("ADD block", "int ADD(int a, int b) {\n   return a + b}");
    blockVector->append(math_add);
    QPair<QString, QString> math_sub("SUB block", "int SUB(int a, int b) {\n   return a - b}");
    blockVector->append(math_sub);
    QPair<QString, QString> math_mul("MUL block", "int MUL(int a, int b) {\n   return a * b}");
    blockVector->append(math_mul);
    QPair<QString, QString> math_div("DIV block", "int DIV(int a, int b) {\n   return a / b}");
    blockVector->append(math_div);
}

void MainWindow::on_blockTree_itemClicked(QTreeWidgetItem *item, int column)
{
    // TODO: add searching in vector
    if (item->text(0) == "ADD block"){
          ui->CodeEditor->setText(blockVector->first().second);
    } else if (item->text(0) == "SUB block") {
        ui->CodeEditor->setText("");
    } else {
        ui->CodeEditor->setText("");
    }
}
