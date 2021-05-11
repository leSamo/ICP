#-------------------------------------------------
#
# Project created by QtCreator 2021-04-30T12:56:37
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ICP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    helpdialog.cpp \
    callback.cpp \
    actionlistener.cpp \
    topicdialog.cpp \
    publisher.cpp

HEADERS  += mainwindow.h \
    helpdialog.h \
    callback.h \
    actionlistener.h \
    topicdialog.h \
    message.h \
    publisher.h

FORMS    += mainwindow.ui \
    helpdialog.ui \
    topicdialog.ui

LIBS += -lpaho-mqttpp3 -lpaho-mqtt3as
