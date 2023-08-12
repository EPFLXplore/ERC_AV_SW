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

#define JETSON_NODE_ID (0x000)
#define SC1_NODE_ID    (0x001)
#define SC2_NODE_ID    (0x002)
#define HD_NODE_ID     (0x004)
#define NAV_NODE_ID    (0x008)


extern CANBus* FDCAN1_network;

#define SET_DESTINATION_NODE_ID(id) dynamic_cast<ROCANDriver*>(FDCAN1_network->get_driver())->TxHeaderConfigID(id);

void setupNodeID();
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
