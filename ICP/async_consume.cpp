// --------------------
//
// --------------------

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mainwindow.h"
#include "async_consume.h"

using namespace std;

const string SERVER_ADDRESS	{ "test.mosquitto.org:1883" };
string topic;

const int N_RETRY_ATTEMPTS = 5;
const int QOS = 2;

int async_consume(string server, callback *eventCallback) {

}
/*
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
void setTopic(string subscribe) {
    topic = subscribe;
}
