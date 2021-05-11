/*! \file publisher.cpp
 * ICP project 2020/21
 * Authors: Samuel Olekšák
 * Parts of this file might have been transferred from eclipse/paho.mqtt.cpp github repository, licence: https://www.eclipse.org/legal/epl-v10.html
 */

#include <iostream>
#include "mqtt/client.h"
#include "publisher.h"

#define CLIENT_ID "xoleksxfindrSender"

/*!
* Construct and publish a message
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
    catch (const mqtt::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
