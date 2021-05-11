/*! \file topicdialog.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */

#include "topicdialog.h"
#include "ui_topicdialog.h"

/*!
* Topic Dialog constructor
* \param[in] parent - parent widget
* \param[in] topic
* \param[in] msgs - deque of all message history of this this topic
*/
TopicDialog::TopicDialog(QWidget *parent, QString topic, std::deque<msg> msgs) : QDialog(parent), ui(new Ui::TopicDialog) {
    ui->setupUi(this);

    this->topic = topic;
    this->setWindowTitle(topic);
    this->setMsgs(msgs);

    // hide row numbers in tablewidget
    ui->tableWidget->verticalHeader()->setVisible(false);
}

TopicDialog::~TopicDialog() {
    delete ui;
}

/*!
* Fill table with messages
* \param[in] msgs - deque of messages to display in the table
*/
void TopicDialog::setMsgs(std::deque<msg> msgs) {
    ui->tableWidget->setRowCount(msgs.size());

    for (int rowIndex = 0; rowIndex < msgs.size(); rowIndex++) {
        QTableWidgetItem *timeCell = new QTableWidgetItem;
        QTableWidgetItem *messageCell = new QTableWidgetItem;

        timeCell->setText(msgs[rowIndex].time);
        messageCell->setText(msgs[rowIndex].content);

        ui->tableWidget->setItem(rowIndex, 0, timeCell);
        ui->tableWidget->setItem(rowIndex, 1, messageCell);
    }
}

/*!
* User clicked send btn - read input field and send
* message to current topic
*/
void TopicDialog::on_btnSend_clicked() {
    std::string msg = ui->inputMsg->toPlainText().toUtf8().constData();

    Publisher *publisher = new Publisher();
    publisher->Publish(topic.toUtf8().constData(), msg);
}

/*!
* User clicked OK btn - close the topic modal
*/
void TopicDialog::on_btnOK_clicked() {
    this->destroy(true);
}
