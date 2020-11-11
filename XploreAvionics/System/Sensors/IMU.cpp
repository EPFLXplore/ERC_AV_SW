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
	this.imuData[0] = bno055_getVectorAccelerometer();
	this.imuData[1] = bno055_getVectorEuler(); // bno055_getVectorQuaternion();
	this.imuData[2] = bno055_getVectorGravity();
	writeToRtosBuffer(this.imuData);
	osDelay(100);
}
