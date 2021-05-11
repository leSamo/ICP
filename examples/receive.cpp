#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "mosquitto.h"

#define SERVER_ADDRESS "broker.emqx.io"
#define SERVER_PORT 1883
#define TOPIC "xoleksxfindr/#"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Invalid number of arguments\n");
		return EXIT_FAILURE;
	}

	if (std::string(argv[1]) == "--help") {
		printf("Usage: ./receive [topic] [number of messages to receive]\n");
		return EXIT_SUCCESS;
	}

	struct mosquitto_message *message;

	mosquitto_lib_init();

	int retval = mosquitto_subscribe_simple(
		&message,
		std::stoi(argv[2]),
		false, 				// want retained
		argv[1],			// topic
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
	for (uint i = 0; i < std::stoi(argv[2]); i++) {
		std::cout << message[i].topic << ": " << (char*) message[i].payload << std::endl;
		mosquitto_message_free_contents(&message[i]);
	}

	free(message);
	mosquitto_lib_cleanup();

	return EXIT_SUCCESS;
}