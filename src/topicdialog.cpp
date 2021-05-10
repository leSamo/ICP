#include "topicdialog.h"
#include "ui_topicdialog.h"

TopicDialog::TopicDialog(QWidget *parent, QString topic) :
    QDialog(parent),
    ui(new Ui::TopicDialog) {
    ui->setupUi(this);
    this->topic = topic;
    setTitle(topic);

    // hide row numbers in tablewidget
    ui->tableWidget->verticalHeader()->setVisible(false);
}

TopicDialog::~TopicDialog() {
    delete ui;
}

void TopicDialog::setTitle(QString title) {
    this->setWindowTitle(title);
}

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

void TopicDialog::on_btnSend_clicked() {
    std::string msg = ui->inputMsg->toPlainText().toUtf8().constData();

    Publisher *publisher = new Publisher();
    publisher->Publish(topic.toUtf8().constData(), msg);
}

void TopicDialog::on_btnOK_clicked() {
    this->destroy(true);
}
