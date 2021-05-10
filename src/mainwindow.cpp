#include <sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "async_consume.h"
#include <thread>
#include "helpdialog.h"
#include "topicdialog.h"
#include <QDebug>
#include "mqtt/async_client.h"
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

#define MAX_MSG_COUNT 1000

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // setup tree widget appearance
    ui->treeWidget->setColumnHidden(3, true);
    ui->treeWidget->setColumnWidth(0, 200);
    ui->treeWidget->setColumnWidth(1, 200);

    // setup tree widget on click event handler
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(on_treewidget_clicked(QTreeWidgetItem*, int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

/*!
* Adds item to treeWidget element on root level
* \param[in] name first column content - topic name
* \param[in] desc second column content - last message received
* \return newly created root level item
*/
QTreeWidgetItem* MainWindow::AddRoot(QString name, QString desc, QString time, QString path) {
    // if element with the same name exists dont create a new one
    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(path, Qt::MatchContains|Qt::MatchRecursive, 3);

    QTreeWidgetItem *newItem;

    if (clist.size() > 0) {
        newItem = clist[0];
    }
    else {
        newItem = new QTreeWidgetItem(ui->treeWidget);
        ui->treeWidget->addTopLevelItem(newItem);
    }

    newItem->setText(0, name);
    newItem->setText(1, desc);
    newItem->setText(2, time);
    newItem->setText(3, path);

    return newItem;
}

/*!
* Attach new item to treeWidget element as child
* \param[in] parent treeWidget item to which the new item should be attached to
* \param[in] name first column content - topic name
* \param[in] desc second column content - last message received
* \return newly created treeWidget item
*/
QTreeWidgetItem* MainWindow::AddChild(QTreeWidgetItem *parent, QString name, QString desc, QString time, QString path) {
    // if element with the same name exists dont create a new one
    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(path, Qt::MatchContains|Qt::MatchRecursive, 3);

    QTreeWidgetItem *newItem;

    if (clist.size() > 0) {
        newItem = clist[0];
    }
    else {
        newItem = new QTreeWidgetItem();

        parent->addChild(newItem);
    }

    newItem->setText(0, name);
    newItem->setText(1, desc);
    newItem->setText(2, time);
    newItem->setText(3, path);

    return newItem;
}

const std::string CLIENT_ID { "xoleksxfindr" };

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
    QTreeWidgetItem* parent;
    stringstream ss;

    time_point<system_clock> now = system_clock::now();
    time_t now_time = system_clock::to_time_t(now);

    auto gmt_time = gmtime(&now_time);
    ss << std::put_time(gmt_time, "%H:%M:%S");
    QString QcurrentTime = QString::fromStdString(ss.str());
    ss.str("");

    msg newMsg = { Qtopic, Qmsg, QcurrentTime };
    msgs.push_front(newMsg);

    if (msgs.size() > MAX_MSG_COUNT) {
        msgs.pop_back();
    }

    while(std::getline(test, segment, '/'))
    {
       seglist.push_back(segment);
    }

    if (seglist.size() == 1) {
        parent = MainWindow::AddRoot(QString::fromStdString(seglist[0]), Qmsg, QcurrentTime, QString::fromStdString(seglist[0]));
    }
    else {
        parent = MainWindow::AddRoot(QString::fromStdString(seglist[0]), "", "", QString::fromStdString(seglist[0]));
    }

    for (int i = 1; i < seglist.size(); i++) {
        copy(seglist.begin(),seglist.begin() + i + 1, ostream_iterator<string>(ss,"/"));
        string path = ss.str();
        path.pop_back(); // remove trailing '/'

        if (i == seglist.size() - 1) {
            parent = MainWindow::AddChild(parent, QString::fromStdString(seglist[i]), Qmsg, QcurrentTime, QString::fromStdString(path));
        }
        else {
            parent = MainWindow::AddChild(parent, QString::fromStdString(seglist[i]), "", "", QString::fromStdString(path));
        }
    }
}

void MainWindow::on_treewidget_clicked(QTreeWidgetItem *item, int column) {
    cout << "Show history for " << item->text(3).toUtf8().constData() << endl;

    QString topic = item->text(3);

    TopicDialog *topicDialog = new TopicDialog(this);

    topicDialog->show();
    topicDialog->setTitle(topic);

    cout << topic.toUtf8().constData() << " vs " << msgs[0].topic.toUtf8().constData() << endl;

    std::deque<msg> filteredMsgs;
    std::copy_if(msgs.begin(), msgs.end(), std::back_inserter(filteredMsgs), [topic](msg message){return message.topic == topic;});
    topicDialog->setMsgs(filteredMsgs);
}
