/*
 * Telemetry.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
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

class Telemetry {
public:
	static void setup();

	static void handle_ping(uint8_t sender_id, PingPacket* packet);

	static void set_id(uint32_t id);
private:
};

extern CANBus* FDCAN1_network;

#endif /* TELEMETRY_TELEMETRY_H_ */
