#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "mosquitto.h"

#define SERVER_ADDRESS "broker.emqx.io"
#define SERVER_PORT 1883
#define MAX_MSG_COUNT 1 // how many messages should be received before printing and existing
#define TOPIC "xoleksxfindr/#"

int main(int argc, char *argv[]) {
	struct mosquitto_message *message;

	mosquitto_lib_init();

	int retval = mosquitto_subscribe_simple(
		&message,
		MAX_MSG_COUNT,
		false, 				// want retained
		TOPIC,
		2, 					// QoS
		SERVER_ADDRESS,
		SERVER_PORT,
		NULL, 				// client id
		60, 				// time to keep alive in seconds
		true,				// clean session
		NULL,				// username
		NULL,				// password
		NULL,				// will
		NULL				// TLS
	);						

	if (retval) {
		std::cerr << "Error: " << mosquitto_strerror(retval) << std::endl;
		mosquitto_lib_cleanup();

		return EXIT_FAILURE;
	}

	// listen for MAX_MSG_COUNT messages and print them
	for (uint i = 0; i < MAX_MSG_COUNT; i++) {
		std::cout << message[i].topic << ": " << (char*) message[i].payload << std::endl;
		mosquitto_message_free_contents(&message[i]);
	}

	free(message);
	mosquitto_lib_cleanup();

	return EXIT_SUCCESS;
}