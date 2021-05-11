/*! \file actionlistener.h
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */

#ifndef ACTIONLISTENER_H
#define ACTIONLISTENER_H

#include "mqtt/properties.h"
#include "mqtt/async_client.h"

class action_listener : public virtual mqtt::iaction_listener {
public:
    action_listener(const std::string& name) : name_(name) {}
private:
    std::string name_;
    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
};

#endif // ACTIONLISTENER_H
