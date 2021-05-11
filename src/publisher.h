/*! \file publisher.h
 * ICP project 2020/21
 * Author: Samuel Olešák
 */


#ifndef PUBLISHER_H
#define PUBLISHER_H

class Publisher {
public:
    static void Publish(std::string serverAddress, std::string topic, std::string content);
};

#endif // PUBLISHER_H
