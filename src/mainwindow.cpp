/*! \file mainwindow.cpp
 * ICP project 2020/21
 * Authors: Samuel Olekšák, Michal Findra
 */

#include <sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"
#include "topicdialog.h"
#include <QDebug>
#include "mqtt/async_client.h"
#include <chrono>
#include <iomanip>
#include <QCloseEvent>
#include <fstream>
#include <filesystem>

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

    // check config file for dashboard widgets
    QString fileName("widgets.json");
    QFile file(fileName);

    // Read JSON config file or create empty
    if(QFileInfo::exists(fileName))
    {
        readJson();
    } else {
        file.open(QIODevice::ReadWrite | QIODevice::Text);        
        file.close();
    }

    // setup tree widget on double click event handler
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(showTopicHistory(QTreeWidgetItem*, int)));

    mqttClient = nullptr;
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
    msgContent != "" ? newItem->setText(1, msgContent) : void();
    timeOfMsgReceived != "" ? newItem->setText(2, timeOfMsgReceived) : void();
    newItem->setText(3, topic);

    return newItem;
}

/*!
* Update value in dash panel if panel for subscribed topic exists
* \param[in] value - displayed value
* \param[in] topic - subscribed topic
*/
void MainWindow::AddToDash(QString value, QString topic){
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());

    // find widget with topic
    for (int y = 0; y < 2; y++){
        for (int x = 0; x < 5;x++){
            if (layout->itemAtPosition(x,y) != 0){
                QList<QLabel *> labels = layout->itemAtPosition(x,y)->widget()->findChildren<QLabel *>();
                if (QString::compare( labels[1]->text() ,("Topic: " + topic)) == 0 && labels.count() == 3){
                   labels[2]->setText("Value: "+ value);
                }
            }
        }
    }
}

void MainWindow::on_btnConnect_clicked() {
    QString server = ui->inputServer->toPlainText();
    QString topic = ui->inputTopic->toPlainText();
    ui->txtTitle->setText(topic);
    serverAddress = server.toUtf8().constData();

    if (isConnected()) {
        disconnect();
    }

    callback *eventCallback;
    mqttClient = nullptr;
    mqtt::connect_options *connectionOptions;

    // we need an async client to be able to listen to msgs on background
    mqttClient = new mqtt::async_client(server.toUtf8().constData(), CLIENT_ID);

    connectionOptions = new mqtt::connect_options();
    eventCallback = new callback(*mqttClient, *connectionOptions, topic.toUtf8().constData());
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

int MainWindow::disconnect() {
    cout << "Disconnecting" << endl;

    try {
        mqttClient->disconnect()->wait();
    }
    catch (const mqtt::exception& e) {
        cout << "Failed to disconnect: " << e << endl;
        return 1;
    }

    cout << "Disconnected" << endl;
    mqttClient = nullptr;

    return 0;
}

bool MainWindow::isConnected() {
    return mqttClient != nullptr;
}

/*!
* Create new widget in Dashboard, from given values and type
*/
void MainWindow::on_btnAdd_item_clicked() {
    // get dashboard layout
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());

    // create new dashboard widget
    QWidget* wdg = new QWidget();
    wdg->setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 3px;  }");
    wdg->setFixedSize(QSize(350, 80));

    // dashboard widget layouts
    QVBoxLayout *layout2 = new QVBoxLayout();
    QHBoxLayout *layout3 = new QHBoxLayout();

    // create and set name and topic label
    QLabel *label_name = new QLabel(wdg);
    QLabel *label_topic = new QLabel(wdg);
    QString str_name = "Name: " + ui->dash_name->toPlainText();
    QString str_topic = "Topic: " + ui->dash_topic->toPlainText();
    label_name->setText(str_name);
    label_topic->setText(str_topic);

    //create buuton to close widget
    QPushButton* button = new QPushButton("X");
    button->setFixedSize(QSize(50, 50));
    button->setStyleSheet("background-color : rgba( 100, 100, 100, 255); ");

    // create Recieve type or Send type widget
    if (QString::compare(ui->dash_CB->currentText(), "Recieve") == 0){

        // create value label
        QLabel *label_type = new QLabel(wdg);
        QString str_type = "Value: ";
        label_type->setText(str_type);

        // insert items into layouts in created widget
        layout2->addWidget(label_name);
        layout2->addWidget(label_topic);
        layout2->addWidget(label_type);
        layout3->addItem(layout2);
        layout3->addWidget(button);

    } else if (QString::compare(ui->dash_CB->currentText(), "Send") == 0){

        // insert items into layouts in created widget
        layout2->addWidget(label_name);
        layout2->addWidget(label_topic);
        QPlainTextEdit *textblock = new QPlainTextEdit(wdg);
        textblock->setStyleSheet("background-color : rgba( 80, 80, 80, 255); ");
        layout2->addWidget(textblock);

        QVBoxLayout *layout4 = new QVBoxLayout();

        // create Send button
        QPushButton* buttonSend = new QPushButton("Send");
        buttonSend->setFixedSize(QSize(50, 20));
        buttonSend->setStyleSheet("background-color : rgba( 100, 100, 100, 255); ");

        // resize close button
        button->setFixedSize(QSize(50, 20));

        // insert items into layouts in created widget
        layout3->addItem(layout2);
        layout4->addWidget(button);
        layout4->addWidget(buttonSend);
        layout3->addItem(layout4);

        // connect Send button behaviour
        QObject::connect(
                    buttonSend, &QPushButton::clicked,
                    this, &MainWindow::onSend);
    }

    // check position in dash grid layout and insert created widget
    wdg->setLayout(layout3);
    for (int y = 0; y < 2; y++){
        for (int x = 0; x < 5;x++){
            if (layout->itemAtPosition(x,y) == 0){
                layout->addWidget(wdg, x, y);
            }
        }
    }

    // connect Close button behaviour
    QObject::connect(
                button, &QPushButton::clicked,
                this, &MainWindow::onRemove);


}

/*!
* Close and remove widget from dashboard
*/
void MainWindow::onRemove() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    delete button->parent();
}


/*!
* Send message to subsribed topic from Send widget in dashboard
*/
void MainWindow::onSend() {
    QPushButton* buttonS = qobject_cast<QPushButton*>(sender());

    // get message from calling object
    QList<QPlainTextEdit *> textEdit = buttonS->parent()->findChildren<QPlainTextEdit *>();
    std::string msg = textEdit[0]->toPlainText().toUtf8().constData();

    // get topic from calling object
    QList<QLabel *> labels = buttonS->parent()->findChildren<QLabel *>();
    QStringList pieces = labels[1]->text().split( "Topic: " );
    QString msgTopic = pieces.value( pieces.length() - 1 );

    // publish message to topic
    Publisher::Publish(serverAddress, msgTopic.toUtf8().constData(), msg);
}

/*!
* Open dialog window with About
*/
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

        while(std::getline(ss, segment, '/')) {
           seglist.push_back(segment);
        }
    }

    if (seglist.size() == 1) {
        parent = MainWindow::AttachOrRefreshNode(nullptr, QString::fromStdString(seglist[0]), Qmsg, QcurrentTime, QString::fromStdString(seglist[0]));
    }
    else {
        parent = MainWindow::AttachOrRefreshNode(nullptr, QString::fromStdString(seglist[0]), "", "", QString::fromStdString(seglist[0]));
    }

    for (long unsigned int i = 1; i < seglist.size(); i++) {
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

    // set value in dashboard widget
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

    // filter only msgs, which are of desired topic
    std::deque<msg> filteredMsgs;
    std::copy_if(msgs.begin(), msgs.end(), std::back_inserter(filteredMsgs), [topic](msg message){std::string content = message.topic.toUtf8().constData(); return content.find(topic.toUtf8().constData()) == 0;});

    TopicDialog *topicDialog = new TopicDialog(this, serverAddress, topic, filteredMsgs);
    topicDialog->show();
}

/*!
* Read Dashboard widgets data from JSON config file widgets.json
*/
void MainWindow::readJson()
{
      // read dashboard widget data from json file
      QString val;
      QFile file;
      file.setFileName("widgets.json");
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();
      }

      // parse json document
      QJsonDocument jsDoc = QJsonDocument::fromJson(val.toUtf8());
      QJsonObject jsObject = jsDoc.object();
      QJsonArray jsArray = jsObject["widgets"].toArray();       

      // insert all widgets from json file
      foreach (const QJsonValue & value, jsArray) {
          QJsonObject obj = value.toObject();
          ui->dash_name->setPlainText((obj["name"].toString()));
          ui->dash_topic->setPlainText((obj["topic"].toString()));
          if (QString::compare(obj["type"].toString(), "Recieve", Qt::CaseInsensitive) == 0){
              ui->dash_CB->setCurrentText("Recieve");               
          } else {
              ui->dash_CB->setCurrentText("Send");              
          }

          ui->btnAdd_item->click();
      }

      // reset textbox text
      ui->dash_name->setPlainText("Name of dash element");
      ui->dash_topic->setPlainText("Topic of dash element");
      ui->dash_CB->setCurrentText("Recieve");
   }


/*!
* Save created widgets into JSON file on exit
* \param[in] event - MainWindow close event
*/
void MainWindow::closeEvent (QCloseEvent *event){
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->dash_g->layout());
    QJsonArray jsArray;

    // save all created dashboard widgets into json object
    for (int y = 1; y >= 0; y--){
        for (int x = 4; x >= 0;x--){
            if (layout->itemAtPosition(x,y) != 0){
                QList<QLabel *> labels = layout->itemAtPosition(x,y)->widget()->findChildren<QLabel *>();
                QJsonObject item_data;
                QStringList pieces = labels[0]->text().split( "Name: " );
                item_data.insert("name", QJsonValue(pieces.value( pieces.length() - 1 )));

                QStringList pieces2 = labels[1]->text().split( "Topic: " );
                item_data.insert("topic", QJsonValue(pieces2.value( pieces2.length() - 1 )));

                if (labels.count() == 2){
                    item_data.insert("type", "Send");
                } else {
                    item_data.insert("type", "Recieve");                   
                }
                 jsArray.push_back(QJsonValue(item_data));
            }
        }
    }

    // put created objects into json list into json document
    QJsonObject final_object;
    final_object.insert(QString("widgets"), QJsonValue(jsArray));
    QJsonDocument document;
    document.setObject(final_object);
    QByteArray bytes = document.toJson( QJsonDocument::Indented );

    // write created json object into json file
    QString fileName("widgets.json");
    QFile file(fileName);

    if ( file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
       QTextStream iStream( &file );
       iStream.setCodec( "utf-8" );
       iStream << bytes;
       file.close();
    }
}

void MainWindow::on_actionSnapshot_triggered() {
    for (size_t i = 0; i < msgs.size(); i++) {
        msg currentMsg = msgs[msgs.size() - i - 1];
        std::string msgTopic = std::string(currentMsg.topic.toUtf8().constData());

        std::filesystem::create_directories("snapshots/" + msgTopic);

        std::ofstream msgFile("./snapshots/" + msgTopic + "/payload.txt");
        msgFile << currentMsg.content.toUtf8().constData() << std::endl;
        msgFile.close();
    }

    cout << "Snapshot written!" << endl;
}
