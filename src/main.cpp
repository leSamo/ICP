/*! \file main.cpp
 * ICP project 2020/21
 * Authors: Samuel Olekšák, Michal Findra
 */

#include "mainwindow.h"
#include <QApplication>

/*! \mainpage MQTT Explorer
 * ICP project 2020/21
 * Samuel Olekšák (xoleks00)
 * Michal Findra (xfindr00)
 *
 * \section Description
 * Application able to connect to MQTT broker and receive either
 * all messages or messages of particular topic.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
