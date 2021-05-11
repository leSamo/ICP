/*! \file actionlistener.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 */

#include "actionlistener.h"

void action_listener::on_failure(const mqtt::token& tokenId) {
    std::cout << "Action " << actionName_ << " failure!";
}

void action_listener::on_success(const mqtt::token& tokenId) {
    std::cout << "Action " << actionName_ << " success!";
}

