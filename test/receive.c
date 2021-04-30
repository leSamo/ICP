#include <stdlib.h>
#include <stdio.h>
#include "mosquitto.h"

#define MAX_MSG_COUNT 5
#define BASE_TOPIC "xoleksxfindr/#"
#define BROKER_ADDRESS "test.mosquitto.org"
#define BROKER_PORT 1883

int main(int argc, char *argv[])
{
	int rc;
	int i;
	struct mosquitto_message *msg;

	mosquitto_lib_init();

	rc = mosquitto_subscribe_simple(
			&msg, MAX_MSG_COUNT, false,
			BASE_TOPIC, 2,
			BROKER_ADDRESS, BROKER_PORT,
			NULL, 60, true,
			NULL, NULL,
			NULL, NULL);

	if (rc) {
		printf("Error: %s\n", mosquitto_strerror(rc));
		mosquitto_lib_cleanup();
		return rc;
	}

	for (i = 0; i < MAX_MSG_COUNT; i++) {
		printf("%s %s\n", msg[i].topic, (char *)msg[i].payload);
		mosquitto_message_free_contents(&msg[i]);
	}
	free(msg);

	mosquitto_lib_cleanup();

	return 0;
}