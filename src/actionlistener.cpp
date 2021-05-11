/*! \file actionlistener.cpp
 * ICP project 2020/21
 * Author: Samuel Olekšák
 * Parts of this file might have been transferred from eclipse/paho.mqtt.cpp github repository, licence: https://www.eclipse.org/legal/epl-v10.html
 */

#include "actionlistener.h"

void action_listener::on_failure(const mqtt::token& tokenId) {
    std::cout << "Action " << actionName_ << " failure!" << std::endl;
}

void action_listener::on_success(const mqtt::token& tokenId) {
    std::cout << "Action " << actionName_ << " success!" << std::endl;
}

