/*
 * Telemetry.h
 *
 *  Created on: Feb 3, 2021
 *      Author: arion
 */

#ifndef TELEMETRY_TELEMETRY_H_
#define TELEMETRY_TELEMETRY_H_


#include "Libraries/RoCo/RoCo.h"

extern BufferedIODriver telemetryDriver;
extern NetworkBus network;


void setupTelemtry();


#endif /* TELEMETRY_TELEMETRY_H_ */
