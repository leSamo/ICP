/*! \file async_consume.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */

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

#endif // ASYNC_CONSUME_H
