/*
 * Telemetry.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */


#include "Telemetry.h"


static uint8_t storage1[256];
static uint8_t storage2[256];
BufferedIODriver telemetryDriver(storage1, storage2, 256);
NetworkBus network(&telemetryDriver);
