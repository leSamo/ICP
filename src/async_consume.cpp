/*! \file async_consume.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */


#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mainwindow.h"
#include "async_consume.h"

/*
int async_consume(string server, callback *eventCallback) {

}

int disconnect() {
    cout << "\nDisconnecting" << endl;

    try {
        mqttClient->disconnect()->wait();
    }
    catch (const mqtt::exception& e) {
        cout << "Failed to disconnect: " << e << endl;
        return 1;
    }

    cout << "Disconnected" << endl;
    mqttClient = nullptr;

    return 0;
}

bool isConnected() {
    return mqttClient == nullptr;
}
*/
