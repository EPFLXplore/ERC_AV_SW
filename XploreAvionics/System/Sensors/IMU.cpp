/*
 * IMU.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#include "IMU.h"

void IMUThread::init() {
	bno055_assignI2C(this->hi2c);
	bno055_setup();
	bno055_setOperationModeNDOF();
}

void IMUThread::loop() {
	bno055_vector_t imuData[3];
	imuData[0] = bno055_getVectorAccelerometer();
	imuData[1] = bno055_getVectorEuler(); // bno055_getVectorQuaternion();
	imuData[2] = bno055_getVectorGravity();
	writeToRtosBuffer(imuData); //envoyer à un autre thread pour etre envoyé par ethernet
	osDelay(100);
}

/*void writeToRtosBuffer(bno055_vector_t imuData[]){

}*/
