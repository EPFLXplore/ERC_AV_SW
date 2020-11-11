/*
 * IMU.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#include "IMU.h"

void IMUThread::init(I2C_HandleTypeDef* hi2c) {
	bno055_assignI2C(hi2c);
	bno055_setup();
	bno055_setOperationModeNDOF();
}

void IMUThread::loop() {
	osDelay(100);
}
