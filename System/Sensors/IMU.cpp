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
	HAL_UART_Transmit(huart, (uint8_t*) "Hello world\n", 12, 50);

	bno055_vector_t imuData[3];
	imuData[0] = bno055_getVectorAccelerometer();
	imuData[1] = bno055_getVectorEuler(); // bno055_getVectorQuaternion();
	imuData[2] = bno055_getVectorGravity();
	char data[256]; //Random size, check this
	int size = sprintf((char *)data, "Acc : ");
	printToUart(huart, (uint8_t*)data, size);
	writeToRtosBuffer(imuData); //envoyer à un autre thread pour etre envoyé par ethernet
	osDelay(100);
}

void IMUThread::writeToRtosBuffer(bno055_vector_t imuData[]){


}

char* IMUThread::bnoVectortoString(bno055_vector_t v){
	char data[256];
	sprintf(data, "%f, %f, %f", v.x, v.y, v.z);
	return data;
}
