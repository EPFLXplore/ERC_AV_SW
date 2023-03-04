/*
 * Prober.cpp
 *
 *  Created on: 8 Apr 2021
 *  Updated on: 13 Aug 2022
 *      Authors: arion, Yassine, Vincent
 *
 */

#include "Prober.h"

#include "i2c.h"

#include "Lang/Operators.h"

#include "Dummy_thread.h"

void ProberThread::init() {
	this->semaphore = xSemaphoreCreateBinary();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	this->i2cNum = checkI2CPort(hi2c);
}

bool ProberThread::probeI2C(uint8_t address) {
	return HAL_I2C_IsDeviceReady(hi2c, address << 1, 3, 100) == HAL_OK;
}

void ProberThread::loop() {
	if (probeI2C(ADS_ADDR_GND)) {
			this->instance = new DummyThread(this);
			xSemaphoreTake(semaphore, portMAX_DELAY);
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