/*! \file callback.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */


#include "callback.h"

const int N_RETRY_ATTEMPTS = 5;
const int QOS = 2;

void callback::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        exit(1);
    }
}

/*!
 * \brief Callback invoked when connection is unsuccessfull
 * \param[in] token
 */
void callback::on_failure(const mqtt::token& token) {
    std::cout << "Failed to connect" << std::endl;

    // try to connect again for a few times
    if (++nretry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

/*!
 * \brief Callback invoked when connection is successfull, same as connected
 * \param[in] token
 */
void callback::on_success(const mqtt::token& token) {}

/*!
 * \brief Callback invoked when connection is successfull
 * \param[in] info - info about event
 */
void callback::connected(const std::string& info) {
    std::cout << "Connected" << std::endl;

    cli_.subscribe(topic_, 2, nullptr, subListener_);
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
 * \param[in] msg - structure containing info about received message
 */
void callback::message_arrived(mqtt::const_message_ptr msg) {
    std::string topic = msg->get_topic();

    if (topic.back() == '/') {
        topic.pop_back();
    }

    emit DisplayMsg(QString::fromStdString(topic), QString::fromStdString(msg->to_string()));

    std::cout << topic << ": " << msg->to_string() << std::endl;
}
