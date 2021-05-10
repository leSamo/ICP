#include "topicdialog.h"
#include "ui_topicdialog.h"

TopicDialog::TopicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopicDialog)
{
    ui->setupUi(this);
}

TopicDialog::~TopicDialog()
{
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
