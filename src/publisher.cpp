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

const std::string SERVER_ADDRESS { "broker.emqx.io:1883" };
const std::string CLIENT_ID { "xoleks" };

const int QOS = 1;

Publisher::Publisher() {

}

void Publisher::Publish(std::string topic, std::string content) {
    mqtt::client client(SERVER_ADDRESS, CLIENT_ID);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    //connOpts.set_clean_session(true);

    try {
        client.connect(connOpts);

        client.publish(mqtt::message(topic, content, QOS, false));

        client.disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}
