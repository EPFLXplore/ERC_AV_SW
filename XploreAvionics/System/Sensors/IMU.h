/*
 * IMU.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include "bno055_stm32.h"
#include "Thread.h"

class IMUThread : Thread {
public:
	IMUThread() : Thread("IMU") {}
	void init(I2C_HandleTypeDef* hi2c);
	void loop();
};

#endif /* SENSORS_IMU_H_ */
