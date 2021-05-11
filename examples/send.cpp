#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <sstream>
#include <iostream>

#include <mosquitto.h>

#define SERVER_ADDRESS "broker.emqx.io"
#define SERVER_PORT 1883
#define MAX_MSG_LENGTH 20
#define DURATION 60 	// for how many seconds send messages

void onConnectionAckReceive(struct mosquitto *mosq, void *obj, int reason_code) {
	printf("%s\n", mosquitto_connack_string(reason_code));

	printf("Client connected.\n");
}

void onMsgPublished(struct mosquitto *mosq, void *obj, int msgId) {
	printf("Published message with id %d\n", msgId);
}

int getRandomInt(int from, int to) {
	time_t t;
	srand((unsigned) time(&t));
	int num = rand() % (from - to + 1) + from;

	return num;
}

// for each line in config file potentially send message
void messageLoop(struct mosquitto *mosq, std::vector<std::string> configList, int second) {
	for (uint i = 0; i < configList.size(); i++) {
		std::string item = configList[i];

		// split line on spaces
		std::vector<std::string> words;
		std::stringstream ss;
		std::string line;

		ss << item;

		while (std::getline(ss, line, ' ')) {
			words.push_back(line);
		}

		std::string topic = words[0];
		std::string interval = words[1];

		// if current time modulo interval is non zero, don't send this message at this second
		if (second % std::stoi(interval) != 0) {
			break;
		}

		std::string message = words[2];

		// generate random integer to be published
		if (message == "RANDOM") {
			int from = std::stoi(words[3]);
			int to = std::stoi(words[4]);

			message = std::to_string(getRandomInt(from, to));
		}

		int retval = mosquitto_publish(mosq, NULL, topic.c_str(), message.length(), message.c_str(), 2, false);

		if (retval) {
			fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(retval));
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Invalid number of arguments\n");
	}
	else if (argv[1] == "--help") {
		printf("Usage: ./send [config-file]\n");
	}

	// initialize and setup mosquitto client
	mosquitto_lib_init();
	struct mosquitto *client = mosquitto_new(NULL, true, NULL);

	if (client == NULL){
		fprintf(stderr, "Error while initializing mosquitto client\n");
		return EXIT_FAILURE;
	}

	// bind callbacks to mqtt client events and then connect
	mosquitto_connect_callback_set(client, onConnectionAckReceive);
	mosquitto_publish_callback_set(client, onMsgPublished);

	int retval = mosquitto_connect(client, SERVER_ADDRESS, SERVER_PORT, 60);

	if (retval){
		mosquitto_destroy(client);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(retval));
		return EXIT_FAILURE;
	}

	retval = mosquitto_loop_start(client);

	if (retval){
		mosquitto_destroy(client);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(retval));
		return EXIT_FAILURE;
	}

	// split config files on newlines into vector
	std::string config;
	std::getline(std::ifstream(argv[1]), config, '\0');

	std::vector<std::string> configList;
	std::stringstream ss;
	std::string line;

	ss << config;

	while (std::getline(ss, line, '\n')) {
		configList.push_back(line);
	}

	// synchronously send messages every second
	for (uint i = 0; i < DURATION; i++) {
		messageLoop(client, configList, i);
		sleep(1);
	}

	mosquitto_disconnect(client);
	mosquitto_lib_cleanup();

	return EXIT_SUCCESS;
}
