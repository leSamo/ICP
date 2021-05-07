#ifndef ASYNC_CONSUME_H
#define ASYNC_CONSUME_H

#include "mqtt/async_client.h"
#include <QMainWindow>
#include "mainwindow.h"
#include "callback.h"

int async_consume(std::string server, callback *eventCallback);
int disconnect();
bool isConnected();
void setTopic(std::string newTopic);
QMainWindow* getMainWindow();

void DisplayMsg(std::string, std::string);


#endif // ASYNC_CONSUME_H
