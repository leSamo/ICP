/*! \file callback.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */


#ifndef CALLBACK_H
#define CALLBACK_H

#include "mqtt/async_client.h"
#include "actionlistener.h"
#include <QObject>

class callback : public QObject, public virtual mqtt::callback, public virtual mqtt::iaction_listener {
    Q_OBJECT
private:
    int nretry_;
    mqtt::async_client& cli_;
    mqtt::connect_options& connOpts_;
    action_listener subListener_;
    std::string topic_;

    void reconnect();

    void on_failure(const mqtt::token& tokenId);
    void on_success(const mqtt::token& tokenId) override;

    void connected(const std::string& reason) override;
    void connection_lost(const std::string& reason) override;
    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    callback(mqtt::async_client& cli, mqtt::connect_options& connOpts, std::string topic)
        : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription"), topic_(topic) {}
signals:
    void DisplayMsg(QString topic, QString msg);
};

#endif // CALLBACK_H
