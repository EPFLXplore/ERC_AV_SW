/*
 * Telemetry.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine
 */

#ifndef TELEMETRY_TELEMETRY_H_
#define TELEMETRY_TELEMETRY_H_


#include "RoCo.h"
#include "usart.h"
#include "fdcan.h"

<<<<<<< Updated upstream:MCU_SOFTWARE/System/Telemetry.h
extern NetworkBus network;
=======
extern NetworkBus FDCAN1_network;
>>>>>>> Stashed changes:Orion_FW/System/Telemetry.h

void setupTelemetry();

#if defined(BUILD_FOR_NAVIGATION)
	extern NetworkBus UART1_network;
#endif

#endif /* TELEMETRY_TELEMETRY_H_ */
