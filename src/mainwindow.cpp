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

/*! How many received messages should be remembered */
#define MAX_MSG_COUNT 1000

/*! MQTT client id */
#define CLIENT_ID "xoleksxfindr"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // setup tree widget appearance
    ui->treeWidget->setColumnHidden(3, true);
    ui->treeWidget->setColumnWidth(0, 200);
    ui->treeWidget->setColumnWidth(1, 200);

    // setup tree widget on double click event handler
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(showTopicHistory(QTreeWidgetItem*, int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

/*!
* Examine whether element with same topic exists, if yes change its content,
* if not create a new treeWidget element
* \param[in] topicSegment first column content
* \param[in] msgContent second column content - content of last message received
* \param[in] timeOfMsgReceived third column content - time of last message received
* \param[in] topic hidden column content - used to search for element
* \return newly created root level item
*/
QTreeWidgetItem* MainWindow::AttachOrRefreshNode(QTreeWidgetItem *parent, QString topicSegment, QString msgContent, QString timeOfMsgReceived, QString topic) {
    // search if element with same topic exists
    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(topic, Qt::MatchContains|Qt::MatchRecursive, 3);


    QTreeWidgetItem *newItem;

    if (clist.size() > 0) {             // exists
        newItem = clist[0];
    }
    else {                              // doesn't exist, create new one
        if (parent) {                   // new non-root level element
            newItem = new QTreeWidgetItem();
            parent->addChild(newItem);
        }
        else {                          // new root level element
            newItem = new QTreeWidgetItem(ui->treeWidget);
            ui->treeWidget->addTopLevelItem(newItem);
        }
    }

    newItem->setText(0, topicSegment);
    newItem->setText(1, msgContent);
    newItem->setText(2, timeOfMsgReceived);
    newItem->setText(3, topic);

    return newItem;
}

/*!
* Update value in dash panel if panel for subscribed topic exists
* \param[in] desc
* \param[in] topic
*/
void MainWindow::AddToDash(QString desc, QString topic){
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());
    for (int y = 0; y < 2; y++){
        for (int x = 0; x < 5;x++){
            if (layout->itemAtPosition(x,y) != 0){
                QList<QLabel *> labels = layout->itemAtPosition(x,y)->widget()->findChildren<QLabel *>();
                if (QString::compare( labels[1]->text() ,("Topic: " + topic)) == 0){
                   labels[2]->setText("Value: "+ desc);
                }
            }
        }
    }
}

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
    std::vector<std::string> seglist;
    QTreeWidgetItem* parent;
    QString QcurrentTime;
    string supertopic;

    {   // get current time in HH:MM:SS format
        stringstream ss;

        time_point<system_clock> now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);

        auto gmt_time = gmtime(&now_time);
        ss << std::put_time(gmt_time, "%H:%M:%S");
        QcurrentTime = QString::fromStdString(ss.str());
    }

    // add received messages to history
    msg newMsg = { Qtopic, Qmsg, QcurrentTime };
    msgs.push_front(newMsg);

    // if buffer is to overflow, remove oldest message
    if (msgs.size() > MAX_MSG_COUNT) {
        msgs.pop_back();
    }

    {   // split topic name on '/' character
        std::stringstream ss(Qtopic.toUtf8().constData());
        std::string segment;

        while(std::getline(ss, segment, '/'))
        {
           seglist.push_back(segment);
        }
    }

    if (seglist.size() == 1) {
        parent = MainWindow::AttachOrRefreshNode(nullptr, QString::fromStdString(seglist[0]), Qmsg, QcurrentTime, QString::fromStdString(seglist[0]));
    }
    else {
        parent = MainWindow::AttachOrRefreshNode(nullptr, QString::fromStdString(seglist[0]), "", "", QString::fromStdString(seglist[0]));
    }

    for (int i = 1; i < seglist.size(); i++) {
        {   // concat this segment and previous segment to form supertopic
            stringstream ss;
            copy(seglist.begin(),seglist.begin() + i + 1, ostream_iterator<string>(ss,"/"));
            supertopic = ss.str();
            supertopic.pop_back(); // remove trailing '/'
        }

        if (i == seglist.size() - 1) {
            parent = MainWindow::AttachOrRefreshNode(parent, QString::fromStdString(seglist[i]), Qmsg, QcurrentTime, QString::fromStdString(supertopic));
        }
        else {
            parent = MainWindow::AttachOrRefreshNode(parent, QString::fromStdString(seglist[i]), "", "", QString::fromStdString(supertopic));
        }        
    }

    MainWindow::AddToDash(Qmsg, Qtopic);
}

/*!
* Event handler for user double clicking a topic.
* Upon user clicking any topic in the treewidget show history for that topic.
* \param[in] item - which row was selected
* \param[in] column - which column was double clicked on - unused
*/
void MainWindow::showTopicHistory(QTreeWidgetItem *item, int column) {
    cout << "Show history for " << item->text(3).toUtf8().constData() << endl;

    QString topic = item->text(3); // row selected by user, topic is extracted by reading column with index 3

    TopicDialog *topicDialog = new TopicDialog(this);

    topicDialog->show();
    topicDialog->setTitle(topic);

    // filter only msgs, which are of desired topic
    std::deque<msg> filteredMsgs;
    std::copy_if(msgs.begin(), msgs.end(), std::back_inserter(filteredMsgs), [topic](msg message){return message.topic == topic;});

    topicDialog->setMsgs(filteredMsgs);
}
