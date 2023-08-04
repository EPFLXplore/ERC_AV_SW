/*
 * Prober.cpp
 *
 *  Created on: 8 Apr 2021
 *  Updated on: Aug 2023
 *      Authors: Arion Zimmermann, Yassine Bakkali, Vincent Nguyen
 *
 */

#include <ADS1234_Mass_thread.hpp>
#include <AS7265_thread.h>
#include "i2c.h"

#include "Lang/Operators.h"
#include "Prober.h"


#include "bmi08_defs.h"
#include "AHRS_thread.h"
#include <Modbus_thread.hpp>
#include "ADS1115_Voltmeter_thread.hpp"
#include "ADS1115_Potentiometer_thread.hpp"


void ProberThread::init() {
	this->semaphore = xSemaphoreCreateBinary();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	this->i2cNum = checkI2CPort(hi2c);

	// I2C scanner
	int i = 0;
	if (hi2c == &hi2c1)
		i = 1;
	else if (hi2c == &hi2c2)
		i = 2;
	else if (hi2c == &hi2c3)
		i = 3;
	for(uint16_t id=1; id<128; id++)
	{
		HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(id<<1), 3, 1000);
		if(ret == HAL_OK)
		{
			printf("Device ID %d found on I2C %d \n", id, i);
		}
	}
}

bool ProberThread::probeI2C(uint8_t address) {
	return HAL_I2C_IsDeviceReady(hi2c, address << 1, 3, 100) == HAL_OK;
}

void ProberThread::loop() {
	if (probeI2C(ADS_ADDR_VDD)) {
			this->instance = new VoltmeterThread(this);
			this->instance->setTickDelay(100);
			xSemaphoreTake(semaphore, portMAX_DELAY);
	}
	if (probeI2C(ADS_ADDR_GND)) {
		this->instance = new PotentiometerThread(this);
		this->instance->setTickDelay(100);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	}
	if (probeI2C(BMI08_ACCEL_I2C_ADDR_PRIMARY) && probeI2C(LIS3MDL_I2CADDR_DEFAULT)){
		this->instance = new AHRSThread(this);
		this->instance->setTickDelay(5); // 200 Hz
		xSemaphoreTake(semaphore, portMAX_DELAY);
	}
	if (probeI2C(AS7265X_ADDR)) {
		this->instance = new AS7265Thread(this);
		this->instance->setTickDelay(1000);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	}


	if (probeI2C(ADS1234_HAT_ADDR)) {
		if (hi2c == &hi2c1)
			this->instance = new ADS1234Thread(this, &hspi1);
		else if (hi2c == &hi2c2)
			this->instance = new ADS1234Thread(this, &hspi2);
		else if (hi2c == &hi2c3)
			this->instance = new ADS1234Thread(this, &hspi3);
		this->instance->setTickDelay(100);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	}
	if (probeI2C(MODBUS_HAT_ADDR)) {
		if (hi2c == &hi2c2) {
			this->instance = new ModbusThread(this);
			this->instance->setTickDelay(1000);
			xSemaphoreTake(semaphore, portMAX_DELAY);
		}
	}
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
