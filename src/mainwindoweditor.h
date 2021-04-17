#ifndef MAINWINDOWEDITOR_H
#define MAINWINDOWEDITOR_H

#include <QMainWindow>
#include <QPainter>

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
private:
    Ui::MainWindowEditor *ui;

};

#endif // MAINWINDOWEDITOR_H
