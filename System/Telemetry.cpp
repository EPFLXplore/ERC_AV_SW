/*
 * Telemetry.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */


#include "Telemetry.h"

BufferedIODriver telemetryDriver;
NetworkBus network(&telemetryDriver);
