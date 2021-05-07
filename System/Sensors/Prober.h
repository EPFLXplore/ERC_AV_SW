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
	ProberThread(I2C_HandleTypeDef* hi2c) : Thread("Prober", 1024), hi2c(hi2c), instance(nullptr), i2cNum(checkI2CPort(hi2c)) {}
	void init();
	void loop();
	void resetProber();
	I2C_HandleTypeDef* getI2C() { return hi2c; }
	char* getI2CNum();

private:
	I2C_HandleTypeDef* hi2c;
	SemaphoreHandle_t semaphore;
	Thread* instance;
	char* i2cNum;
	char* checkI2CPort(I2C_HandleTypeDef* hi2c);
	bool probeI2C(uint8_t address);
	bool probeDB();
};


#endif /* SENSORS_PROBER_H_ */
