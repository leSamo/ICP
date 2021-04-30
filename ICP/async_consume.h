#ifndef ASYNC_CONSUME_H
#define ASYNC_CONSUME_H

#include "mqtt/async_client.h"

int async_consume(std::string server);
int disconnect();
bool isConnected();
void setTopic(std::string newTopic);

#endif // ASYNC_CONSUME_H
