/*! \file topicdialog.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */


#ifndef TOPICDIALOG_H
#define TOPICDIALOG_H

#include <QDialog>
#include <deque>
#include "message.h"
#include "publisher.h"

namespace Ui {
class TopicDialog;
}

class TopicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopicDialog(QWidget *parent, QString topic, std::deque<msg> msgs);
    ~TopicDialog();

private slots:
    void on_btnSend_clicked();
    void on_btnOK_clicked();

private:
    Ui::TopicDialog *ui;
    QString topic;

    void setMsgs(std::deque<msg> msgs);
};

#endif // TOPICDIALOG_H
