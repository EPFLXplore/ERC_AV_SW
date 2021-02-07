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
	IMUThread(I2C_HandleTypeDef* hi2c) : Thread("IMU"), hi2c(hi2c) {}
	void init();
	void loop();

private:
	I2C_HandleTypeDef* hi2c;
	Vector bnoVectorToVector(bno055_vector_t v);
};

#endif /* SENSORS_IMU_H_ */
