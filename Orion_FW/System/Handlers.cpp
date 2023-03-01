#include "Handlers.h"
#include "Telemetry.h"
#include "stdio.h"

void handle_dummyCallback(uint8_t sender_id, DummySystem_DummyPacket* packet) {
	printf("Dummy packet received from [%d] with value %d", sender_id, packet->data);
}
