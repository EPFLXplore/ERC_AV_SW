#include "Handlers.h"
#include "Telemetry.h"
#include "stdio.h"

void handle_dummyCallback(uint8_t sender_id, DummyPacket* packet) {
	printf("Dummy packet received from [%d] with value %d \n", sender_id, packet->data);
}
