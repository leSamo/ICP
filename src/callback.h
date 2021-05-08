#ifndef CALLBACK_H
#define CALLBACK_H

#include "mqtt/async_client.h"
#include "actionlistener.h"
#include <QObject>

class callback : public QObject, public virtual mqtt::callback, public virtual mqtt::iaction_listener {
    Q_OBJECT
private:
    // Counter for the number of connection retries
    int nretry_;
    // The MQTT client
    mqtt::async_client& cli_;
    // Options to use if we need to reconnect
    mqtt::connect_options& connOpts_;
    // An action listener to display the result of actions.
    action_listener subListener_;

    void reconnect();

    // Re-connection failure
    void on_failure(const mqtt::token& tok);

    // (Re)connection success
    // Either this or connected() can be used for callbacks.
    void on_success(const mqtt::token& tok) override;

    // (Re)connection success
    void connected(const std::string& cause) override;

    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const std::string& cause) override;
    // Callback for when a message arrives.
    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
        : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
signals:
    void DisplayMsg(QString topic, QString msg);
};

#endif // CALLBACK_H
