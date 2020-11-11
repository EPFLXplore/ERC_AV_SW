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
private:
	writeToRtosBuffer(); //IMPLEMENT ME : une fonction qui envoie directement les valeurs par ETHERNET ou qui met les valeurs sur un buffer pour les envoyer par ETHERNET?
	bno055_vector_t imuData[3];
};

#endif /* SENSORS_IMU_H_ */
