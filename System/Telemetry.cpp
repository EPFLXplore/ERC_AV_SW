/*
 * Telemetry.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */


#include "Telemetry.h"

#include "Debug/Debug.h"

static volatile uint8_t* control = (uint8_t*) 0x38001000;
static volatile uint8_t* storage = (uint8_t*) 0x38001001;

BufferedIODriver telemetryDriver(storage, control, 256); // Point to RAM D3
NetworkBus network(&telemetryDriver);



void setupTelemtry() {
	network.forward<PingPacket>(&network);
}
