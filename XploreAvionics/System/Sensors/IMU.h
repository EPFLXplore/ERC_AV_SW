/*
 * IMU.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include "Thread.h"

class IMUThread : Thread {
public:
	IMUThread() : Thread("IMU") {}
	void init();
	void loop();
};

#endif /* SENSORS_IMU_H_ */
