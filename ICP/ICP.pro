#-------------------------------------------------
#
# Project created by QtCreator 2021-04-30T12:56:37
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ICP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    async_consume.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    async_consume.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    helpdialog.ui

LIBS += -lpaho-mqttpp3 -lpaho-mqtt3as

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Downloads/paho.mqtt.cpp/build/src/release/ -lpaho-mqttpp3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Downloads/paho.mqtt.cpp/build/src/debug/ -lpaho-mqttpp3
else:unix: LIBS += -L$$PWD/../../Downloads/paho.mqtt.cpp/build/src/ -lpaho-mqttpp3

INCLUDEPATH += $$PWD/../../Downloads/paho.mqtt.cpp/build/src
DEPENDPATH += $$PWD/../../Downloads/paho.mqtt.cpp/build/src

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Downloads/paho.mqtt.c/build/src/release/ -lpaho-mqtt3as
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Downloads/paho.mqtt.c/build/src/debug/ -lpaho-mqtt3as
else:unix: LIBS += -L$$PWD/../../Downloads/paho.mqtt.c/build/src/ -lpaho-mqtt3as

INCLUDEPATH += $$PWD/../../Downloads/paho.mqtt.c/build/src
DEPENDPATH += $$PWD/../../Downloads/paho.mqtt.c/build/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/paho.mqtt.c/build/src/release/libpaho-mqtt3as.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/paho.mqtt.c/build/src/debug/libpaho-mqtt3as.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/paho.mqtt.c/build/src/release/paho-mqtt3as.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/paho.mqtt.c/build/src/debug/paho-mqtt3as.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Downloads/paho.mqtt.c/build/src/libpaho-mqtt3as.a
