/*
 * IMU.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include "Thread.h"
#include "i2c.h"
#include "../Drivers/Sensors/bno055_stm32.h"

class IMUThread : Thread {
public:
	IMUThread(I2C_HandleTypeDef* hi2c) : Thread("IMU"), hi2c(hi2c) {}
	void init();
	void loop();
private:
	I2C_HandleTypeDef* hi2c;
	void writeToRtosBuffer(bno055_vector_t imuData[]); //IMPLEMENT ME : une fonction qui met les valeurs sur un buffer pour les envoyer par ETHERNET
};

#endif /* SENSORS_IMU_H_ */
