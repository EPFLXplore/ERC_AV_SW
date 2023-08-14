/*
 * System.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


#include "Telemetry.h"
//#include "i2c.h"

class System {
public:
	static void init();
	static void prioritize_3V3_LDO();
	static void prioritize_3V3_buck();
	static void enable_meissa_5V_output();
	static void disable_meissa_5V_output();
	static void enable_meissa_3V3_output();
	static void disable_meissa_3V3_output();
	static void update_node_id();
	static uint32_t get_node_id();

	static void handle_meissa_output_cmd(uint8_t sender_id, LaserPacket* packet);

private:
	static uint32_t node_id;
};


#endif /* SYSTEM_H_ */
