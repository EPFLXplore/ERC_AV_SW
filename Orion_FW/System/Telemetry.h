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

<<<<<<< HEAD:MCU_SOFTWARE/System/Telemetry.h
<<<<<<< Updated upstream:MCU_SOFTWARE/System/Telemetry.h
extern NetworkBus network;
=======
extern NetworkBus FDCAN1_network;
>>>>>>> Stashed changes:Orion_FW/System/Telemetry.h
=======
//extern NetworkBus network;
>>>>>>> 15e8edf9b101a619741313a75427759da7f1b1e7:Orion_FW/System/Telemetry.h

void setupTelemetry();

#if defined(BUILD_FOR_NAVIGATION)
//	extern NetworkBus UART1_network;
#endif

//extern NetworkBus JetsonNetwork;
//extern NetworkBus UART3_network;
//extern NetworkBus UART4_network;
//extern NetworkBus UART5_network;
//extern NetworkBus UART8_network;

#endif /* TELEMETRY_TELEMETRY_H_ */
