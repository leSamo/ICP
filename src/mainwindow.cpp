#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);   
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
