/*! \file callback.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */

#include "callback.h"

/*!
 * \brief Callback invoked when connection is successfull
 * On connection success subscribe to desired topic
 * \param[in] info - info about event
 */
void callback::connected(const std::string& info) {
    std::cout << "Connected: " << info << std::endl;

    client_.subscribe(topic_, 2, nullptr, subscriptionListener_);
}

/*!
 * \brief Callback invoked when connection to MQTT server is lost
 * \param[in] cause - info about event
 */
void callback::connection_lost(const std::string& reason) {
    std::cout << "Connection lost: " << reason << std::endl;
}

/*!
 * \brief Callback invoked when a message arrives
 * Signal to main window to display a new message
 * \param[in] msg - structure containing info about received message
 */
void callback::message_arrived(mqtt::const_message_ptr msg) {
    std::string topic = msg->get_topic();

    // remove trailing slash on a topic name
    if (topic.back() == '/') {
        topic.pop_back();
    }

    emit DisplayMsg(QString::fromStdString(topic), QString::fromStdString(msg->to_string()));

    std::cout << topic << ": " << msg->to_string() << std::endl;
}

/*!
 * \brief Callback invoked when connection is unsuccessfull
 * \param[in] token
 */
void callback::reconnect() {
    try {
        // wait for a second before retrying
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        client_.connect(connectionOptions_, nullptr, *this);
    }
    catch (const mqtt::exception& e) {
        std::cout << "Error while connecting: " << e.what() << std::endl;
        exit(1);
    }
}

/*!
 * \brief Callback invoked when connection is unsuccessfull
 * \param[in] token
 */
void callback::on_failure(const mqtt::token& token) {
    std::cout << "Failed to connect" << std::endl;

    // try to connect again for 3 times, fail if unable to
    if (--timesToRetry_ == 0)
        exit(1);
    reconnect();
}
