/*
 * Prober.cpp
 *
 *  Created on: 8 Apr 2021
 *  Updated on: 13 Aug 2022
 *      Authors: arion, Yassine, Vincent
 *
 */


#include <max11615.h>

#include "i2c.h"

#include "Lang/Operators.h"
#include "Prober.h"


#include "Dummy_thread.h"
#include "MAX11615_thread.h"
#include "ADS1234_thread.hpp"
#include "bmi08_defs.h"
#include "lis3mdl_sens.hpp"
#include "IMU_thread.h"
#include "dummy_addresses.h"
#include "AS7265_thread.h"
#include "all_in_one_thread.h"


void ProberThread::init() {
	this->semaphore = xSemaphoreCreateBinary();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	this->i2cNum = checkI2CPort(hi2c);

	this->instance = new ADS1234Thread(this, &hspi2);
//	this->instance = new AllInOneThread();
//	xSemaphoreTake(semaphore, portMAX_DELAY);
}

bool ProberThread::probeI2C(uint8_t address) {
	return HAL_I2C_IsDeviceReady(hi2c, address << 1, 3, 100) == HAL_OK;
}

void ProberThread::loop() {
//	if(probeI2C(0x33)){
//	if(true){
//		this->instance = new DummyThread(this);
//		xSemaphoreTake(semaphore, portMAX_DELAY);
//
//	}

//	if (probeI2C(ADDRESS_MAX11615)) {
//			this->instance = new VoltmeterThread(this);
//			xSemaphoreTake(semaphore, portMAX_DELAY);
//	}
//	if (probeI2C(ADS_ADDR_GND)) {
//			this->instance = new DummyThread(this);
//			xSemaphoreTake(semaphore, portMAX_DELAY);
//	}
//	if (probeI2C(BMI08_ACCEL_I2C_ADDR_PRIMARY) && probeI2C(LIS3_I2C_ADDR)){
//		this->instance = new IMUThread(this);
//		xSemaphoreTake(semaphore, portMAX_DELAY);
//	}
//	if (probeI2C(AS7265X_ADDR)) {
//		this->instance = new AS7265Thread(this);
//		xSemaphoreTake(semaphore, portMAX_DELAY);
//	}
//	if (probeI2C(ADD_RS485TRANS)){
//		this->instance = new AllInOneThread();
//		xSemaphoreTake(semaphore, portMAX_DELAY);
//	}

	HAL_I2C_DeInit(hi2c);
	HAL_I2C_Init(hi2c);
}

void ProberThread::resetProber() {
	xSemaphoreGive(semaphore);
}


uint8_t ProberThread::checkI2CPort(I2C_HandleTypeDef* hi2c){
	if(hi2c->Instance==I2C1)
		return 1;
	else if(hi2c->Instance==I2C2)
		return 2;
	else if(hi2c->Instance==I2C3)
		return 3;
	else if(hi2c->Instance==I2C4)
		return 4;
	else
		return -1;
}
