/*! \file callback.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 * Parts of this file might have been transferred from eclipse/paho.mqtt.cpp github repository, licence: https://www.eclipse.org/legal/epl-v10.html
 */


#ifndef CALLBACK_H
#define CALLBACK_H

#include "mqtt/async_client.h"
#include "actionlistener.h"
#include <QObject>

class callback : public QObject, public virtual mqtt::callback, public virtual mqtt::iaction_listener {
    Q_OBJECT
private:
    int timesToRetry_;
    std::string topic_;
    mqtt::async_client& client_;
    mqtt::connect_options& connectionOptions_;
    action_listener subscriptionListener_;

    void reconnect();

    void connected(const std::string& reason) override;
    void connection_lost(const std::string& reason) override;
    void message_arrived(mqtt::const_message_ptr msg) override;

    void on_failure(const mqtt::token& tokenId);
    void on_success(const mqtt::token& tokenId) {};

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    callback(mqtt::async_client& client, mqtt::connect_options& connectionOptions, std::string topic)
        : timesToRetry_(3), client_(client), connectionOptions_(connectionOptions), subscriptionListener_("Subscription"), topic_(topic) {}
signals:
    void DisplayMsg(QString topic, QString msg);
};

#endif // CALLBACK_H
