/*
 * System.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Yassine, Vincent
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
	static bool using_USB_5V();
	static bool using_LDO_3V3();

	static void enable_meissa_5V_output();
	static void disable_meissa_5V_output();
	static void enable_meissa_3V3_output();
	static void disable_meissa_3V3_output();

	static void enable_yellow_led();
	static void disable_yellow_led();

	static void blink_yellow_led(uint8_t times = 3, float freq = 6);

	static void enable_status_led_fdcan1();
	static void disable_status_led_fdcan1();
	static void enable_status_led_fdcan2();
	static void disable_status_led_fdcan2();

	static void enable_silent_mode_fdcan1();
	static void disable_silent_mode_fdcan1();
	static void enable_silent_mode_fdcan2();
	static void disable_silent_mode_fdcan2();

	static void update_node_id();
	static uint32_t get_node_id();

	static void handle_meissa_output_cmd(uint8_t sender_id, LaserPacket* packet);

private:
	static uint16_t node_id;
};


#endif /* SYSTEM_H_ */
