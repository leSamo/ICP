#ifndef TOPICDIALOG_H
#define TOPICDIALOG_H

#include <QDialog>
#include <deque>
#include "message.h"

namespace Ui {
class TopicDialog;
}

class TopicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopicDialog(QWidget *parent = 0);
    void setTitle(QString title);
    void setMsgs(std::deque<msg> msgs);
    ~TopicDialog();

private:
    Ui::TopicDialog *ui;
};

#endif // TOPICDIALOG_H
