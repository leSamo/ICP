/*! \file publisher.cpp
 * ICP project 2020/21
 * Authors: Samuel Olekšák
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include "mqtt/async_client.h"
#include "mqtt/client.h"
#include "publisher.h"

#define CLIENT_ID "xoleks"

/*!
* Publish a message
* \param[in] serverAddress
* \param[in] topic
* \param[in] content - message to publish
*/
void Publisher::Publish(std::string serverAddress, std::string topic, std::string content) {
    mqtt::client client(serverAddress, CLIENT_ID);
    mqtt::connect_options connOpts;

    try {
        client.connect(connOpts);
        client.publish(mqtt::message(topic, content, 2, false));
        client.disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cout << exc.what() << std::endl;
    }
}
