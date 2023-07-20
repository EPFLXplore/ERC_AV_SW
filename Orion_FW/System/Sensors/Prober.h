/*
 * Prober.h
 *
 *  Created on: 8 Apr 2021
 *      Author: arion
 */

#ifndef SENSORS_PROBER_H_
#define SENSORS_PROBER_H_

#include "Thread.h"
#include <FreeRTOS.h>
#include <stdio.h>
#include <string.h>


class ProberThread : Thread {
public:
	ProberThread(I2C_HandleTypeDef* hi2c) : Thread("Prober", osPriorityBelowNormal), hi2c(hi2c), instance(nullptr), i2cNum(0) {}
	void init();
	void loop();
	void resetProber();
	I2C_HandleTypeDef* getI2C() { return hi2c; }
	uint8_t getI2CNum() {return i2cNum; }

private:
	I2C_HandleTypeDef* hi2c;
	xSemaphoreHandle semaphore;
	Thread* instance;
	uint8_t i2cNum;
	uint8_t checkI2CPort(I2C_HandleTypeDef* hi2c);
	bool probeI2C(uint8_t address);
};


#endif /* SENSORS_PROBER_H_ */
