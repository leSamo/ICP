#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BASE_TOPIC "xoleksxfindr/"
#define MAX_MSG_LENGTH 20
#define BROKER_ADDRESS "broker.emqx.io"
#define BROKER_PORT 1883

void onConnectionAckReceive(struct mosquitto *mosq, void *obj, int reason_code)
{
	printf("%s\n", mosquitto_connack_string(reason_code));

	printf("Client connected.\n");
}

void onMsgPublished(struct mosquitto *mosq, void *obj, int msgId)
{
	printf("Published message with id %d\n", msgId);
}

int temperatureSensor(char *buffer)
{
	int temperature = 15 + rand() % 10;

	return snprintf(buffer, sizeof(buffer), "%d", temperature);
}

int humiditySensor(char *buffer)
{
	int humidity = 60 + rand() % 20;

	return snprintf(buffer, sizeof(buffer), "%d%%", humidity);
}


void sendMsgs(struct mosquitto *mosq)
{
	char *buffer = malloc(MAX_MSG_LENGTH);

	int msgLength = temperatureSensor(buffer);

	int retval = mosquitto_publish(mosq, NULL, BASE_TOPIC"temperature", msgLength, buffer, 2, false);

	if (retval) {
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(retval));
	}

	msgLength = humiditySensor(buffer);

	retval = mosquitto_publish(mosq, NULL, BASE_TOPIC"humidity", msgLength, buffer, 2, false);

	if (retval) {
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(retval));
	}

	sleep(1);

	free(buffer);
}


int main(int argc, char *argv[])
{
	struct mosquitto *mosq;
	int retval;

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	mosquitto_connect_callback_set(mosq, onConnectionAckReceive);
	mosquitto_publish_callback_set(mosq, onMsgPublished);

	retval = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);

	if (retval){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(retval));
		return 1;
	}

	retval = mosquitto_loop_start(mosq);
	if (retval){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(retval));
		return 1;
	}

	for (int i = 0; i < 20; i++) {
		sendMsgs(mosq);
	}

	mosquitto_disconnect(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
