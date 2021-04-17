#include "mainwindoweditor.h"
#include "ui_mainwindoweditor.h"

MainWindowEditor::MainWindowEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowEditor)
{   
    ui->setupUi(this);

}

MainWindowEditor::~MainWindowEditor()
{
    delete ui;
}

void MainWindowEditor::paintEvent(QPaintEvent *event)
{
    QPainter myline(this);
    QPen linePen(Qt::gray);

    QSize size = this->size();

    linePen.setWidth(1);
    myline.setPen(linePen);

    for (auto x = 0; x < size.width() ; x+= 12) {
        myline.drawLine(x,0,x,size.height());
    }

    for (auto y = 0; y < size.height() ; y+= 12) {
        myline.drawLine(0,y,size.width(), y);
    }




}
