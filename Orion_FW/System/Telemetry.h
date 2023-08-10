/*
 * Telemetry.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine
 */

#ifndef TELEMETRY_TELEMETRY_H_
#define TELEMETRY_TELEMETRY_H_

#define BUILD_WITH_FDCAN


#include "RoCo.h"
#include "usart.h"
#include "fdcan.h"


extern CANBus FDCAN1_network;

//extern NetworkBus network;

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
