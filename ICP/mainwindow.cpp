#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "async_consume.h"
#include <thread>
#include "helpdialog.h"
#include <QDebug>

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

void MainWindow::on_btnAdd_item_clicked()
{

    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());


         // TODO: value check

        QString str = ui->dash_name->toPlainText();
        str.append("\nvalue:");
        QPushButton* button = new QPushButton(str);

        for (int x = 0; x < 5;x++){
            for (int y = 0; y < 3; y++){
                if (layout->itemAtPosition(x,y) == 0){
                    layout->addWidget(button, x, y);
                }
            }
        }


        QObject::connect(
                    button, &QPushButton::clicked,
                    this, &MainWindow::onRemove);
    }





void MainWindow::onRemove(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    delete button;

}

void MainWindow::on_actionHelp_triggered()
{
    help = new HelpDialog(this);
    help->show();
}
