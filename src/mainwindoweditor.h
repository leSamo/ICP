#pragma once

#ifndef MAINWINDOWEDITOR_H
#define MAINWINDOWEDITOR_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsScene>
#include <QObject>

#include "ui_mainwindoweditor.h"

class GraphNode;

namespace Ui {
class MainWindowEditor;
}

class MainWindowEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowEditor(QWidget *parent = nullptr);
    ~MainWindowEditor();
    virtual void paintEvent(QPaintEvent *event);
    GraphNode* AddNode(const QString& rName, const QPointF& rPosition);
private:
    Ui::MainWindowEditor *ui;
    QGraphicsScene* mpScene = nullptr;

private Q_SLOTS:

    void Slot_BtnAddNode();

private:

    Ui::MainWindowEditor mUI;

    QVector<GraphNode*> mGraphNodes;

};

#endif // MAINWINDOWEDITOR_H
