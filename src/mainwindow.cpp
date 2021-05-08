#include <sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "async_consume.h"
#include <thread>
#include "helpdialog.h"
#include <QDebug>
#include "mqtt/async_client.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

QTreeWidgetItem* MainWindow::AddRoot(QString name, QString desc) {
    // if element with the same name exists dont create a new one
    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(name, Qt::MatchContains|Qt::MatchRecursive, 0);

    if (clist.size() > 0) {
        return clist[0];
    }

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0, name);
    itm->setText(1, desc);

    ui->treeWidget->addTopLevelItem(itm);

    return itm;
}

QTreeWidgetItem* MainWindow::AddChild(QTreeWidgetItem *parent, QString name, QString desc) {
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, name);
    itm->setText(1, desc);

    parent->addChild(itm);

    return itm;
}

const std::string CLIENT_ID		{ "xoleksxfindr" };

void MainWindow::on_btnConnect_clicked() {
    QString server = ui->inputServer->toPlainText();
    QString topic = ui->inputTopic->toPlainText();
    ui->txtTitle->setText(topic);
/*
    if (isConnected()) {
        disconnect();
    }
*/

    callback *eventCallback;
    mqtt::async_client *mqttClient = nullptr;
    mqtt::connect_options *connectionOptions;

    // we need an async client to be able to listen to msgs on background
    mqttClient = new mqtt::async_client(server.toUtf8().constData(), CLIENT_ID);

    connectionOptions = new mqtt::connect_options();
    eventCallback = new callback(*mqttClient, *connectionOptions);
    (*mqttClient).set_callback(*eventCallback);

    QObject::connect(
                eventCallback, &callback::DisplayMsg,
                this, &MainWindow::DisplayMsg);

    std::cout << "Listening to topic: " << topic.toUtf8().constData() << std::endl;
    std::cout << "Connecting" << std::endl;

    try {
        (*mqttClient).connect(*connectionOptions, nullptr, *eventCallback);
    }
    catch (const mqtt::exception& e) {
        std::cout << "\nConnection failed: " << e << std::endl;
    }

}

void MainWindow::on_btnAdd_item_clicked() {
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());

    // TODO: value check

    QWidget* wdg = new QWidget();
    wdg->setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 3px;  }");
    wdg->setFixedSize(QSize(300, 80));
    QLabel *label_name = new QLabel(wdg);
    QLabel *label_topic = new QLabel(wdg);
    QLabel *label_type = new QLabel(wdg);

    QString str_name = "Name: " + ui->dash_name->toPlainText();
    QString str_topic = "Topic: " + ui->dash_topic->toPlainText();
    QString str_type = "Type: ";


    label_name->setText(str_name);
    label_topic->setText(str_topic);
    label_type->setText(str_type);

    QVBoxLayout *layout2 = new QVBoxLayout();
    QHBoxLayout *layout3 = new QHBoxLayout();

    wdg->setLayout(layout3);
    layout2->addWidget(label_name);
    layout2->addWidget(label_topic);
    layout2->addWidget(label_type);

    QPushButton* button = new QPushButton("X");
    button->setFixedSize(QSize(50, 50));
    button->setStyleSheet("background-color : rgba( 100, 100, 100, 255); ");

    layout3->addItem(layout2);
    layout3->addWidget(button);

    for (int y = 0; y < 2; y++){
        for (int x = 0; x < 5;x++){
            if (layout->itemAtPosition(x,y) == 0){
                layout->addWidget(wdg, x, y);
            }
        }
    }

    QObject::connect(
                button, &QPushButton::clicked,
                this, &MainWindow::onRemove);
}

void MainWindow::onRemove() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    delete button->parent();
}

void MainWindow::on_actionHelp_triggered() {
    help = new HelpDialog(this);
    help->show();
}

void MainWindow::DisplayMsg(QString Qtopic, QString Qmsg) {
    // split topic name on '/' character
    std::stringstream test(Qtopic.toUtf8().constData());
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, '/'))
    {
       seglist.push_back(segment);
    }

    QTreeWidgetItem* parent = MainWindow::AddRoot(QString::fromStdString(seglist[0]), Qmsg);

    for (int i = 1; i < seglist.size(); i++) {
        parent = MainWindow::AddChild(parent, QString::fromStdString(seglist[i]), Qmsg);
    }
}
