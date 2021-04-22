#include <QDialog>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>

#include <QDebug>
#include "mainwindoweditor.h"
#include "ui_mainwindoweditor.h"
#include "GraphNode.h"
#include "GraphNodeSlot.h"
#include "GraphGizmo.h"

MainWindowEditor::MainWindowEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowEditor),
    mpScene(new QGraphicsScene(this))
{   
    ui->setupUi(this);
    ui->graphicsView->setScene(mpScene);
    ui->graphicsView->setSceneRect(0, 0, 4096, 4096);

    auto pNodeA = this->AddNode("Test A", QPointF(-200, -200));
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
GraphNode* MainWindowEditor::AddNode(const QString& rName, const QPointF& rPosition)
{
    auto pNode = new GraphNode(rName, rPosition);

    mpScene->addItem(pNode);

    return pNode;
}

void MainWindowEditor::Slot_BtnAddNode()
{
}
