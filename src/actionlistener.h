/*! \file actionlistener.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 * Parts of this file might have been transferred from eclipse/paho.mqtt.cpp github repository, licence: https://www.eclipse.org/legal/epl-v10.html
 */

#ifndef ACTIONLISTENER_H
#define ACTIONLISTENER_H

#include "mqtt/properties.h"
#include "mqtt/async_client.h"

class action_listener : public virtual mqtt::iaction_listener {
public:
    action_listener(const std::string& actionName) : actionName_(actionName) {}
private:
    std::string actionName_;

    void on_success(const mqtt::token& tok) override;
    void on_failure(const mqtt::token& tok) override;
};

#endif // ACTIONLISTENER_H
