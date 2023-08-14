/*
 * monitor.h
 *
 *  Created on: 5 Sep 2020
 *      Author: AV Team 2020
 */

#ifndef APPLICATION_HOSTBOARD_INC_DEBUG_MONITOR_H_
#define APPLICATION_HOSTBOARD_INC_DEBUG_MONITOR_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_MONITORS 32

#define IMU_MONITOR	 		1
#define ACCEL_MONITOR		2
#define GYRO_MONITOR		3
#define MAG_MONITOR			4
#define QUAT_MONITOR		5
#define RPY_MONITOR			6
#define POT_MONITOR			7
#define VOLTMETER_MONITOR	8
#define MASS_MONITOR		9
#define SPECTRO_MONITOR		10
#define FOURINONE_MONITOR	11
#define NPK_MONITOR			12

// Calibration monitors
#define MAG_CAL_MONITOR 	30
#define ACC_CAL_MONITOR 	31

// Plot monitors
#define IMU_PLOT_MONITOR	32


class Monitor {
public:
	bool enter(uint8_t id);
	bool exit(uint8_t id);

	void enable(uint8_t id, uint8_t location, uint8_t refresh_rate);
	void disable(uint8_t id);

	bool is_enabled(uint8_t id);
	uint8_t get_location(uint8_t id);
	uint8_t get_refresh_rate(uint8_t id);
private:
	uint8_t locations[MAX_MONITORS];
	uint8_t monitor_refresh_rate[MAX_MONITORS];
	uint32_t last_update[MAX_MONITORS];
};


#endif /* APPLICATION_HOSTBOARD_INC_DEBUG_MONITOR_H_ */
