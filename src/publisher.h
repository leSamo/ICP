/*! \file publisher.h
 * ICP project 2020/21
 * Author: Samuel Olešák
 * Parts of this file might have been transferred from eclipse/paho.mqtt.cpp github repository, licence: https://www.eclipse.org/legal/epl-v10.html
 */


#ifndef PUBLISHER_H
#define PUBLISHER_H

class Publisher {
public:
    static void Publish(std::string serverAddress, std::string topic, std::string content);
};

#endif // PUBLISHER_H
