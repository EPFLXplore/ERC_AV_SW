/*
 * IMU.h
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_


#include "Thread.h"
#include "DataStructures.h"

#include "Libraries/BNO055/bno055_stm32.h"
#include "Libraries/BNO055/bno055.h"

class IMUThread : Thread {
public:
	IMUThread(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart) : Thread("IMU"), hi2c(hi2c), huart(huart) {}
	void init();
	void loop();

private:
	I2C_HandleTypeDef* hi2c;
	UART_HandleTypeDef* huart;
	char* bnoVectorToString(bno055_vector_t v, char* buffer);
	Vector bnoVectorToVector(bno055_vector_t v);
	void writeToRtosBuffer(IMUData data); //IMPLEMENT ME : une fonction qui met les valeurs sur un buffer pour les envoyer par ETHERNET
};

#endif /* SENSORS_IMU_H_ */
