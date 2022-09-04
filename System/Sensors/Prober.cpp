/*
 * Prober.cpp
 *
 *  Created on: 8 Apr 2021
 *  Updated on: 13 Aug 2022
 *      Authors: arion, AV2022
 *
 */

#include "Prober.h"
#include "IMU.h"
#include "Stemma_thread.h"
#include "TOF_thread.h"
#include "Voltmeter_thread.h"
#include "HX711_thread.h"
#include "INA219_thread.h"
#include "main.h"
//#include "ADC24.h"
//#include "ADC16.h"

#include "i2c.h"

#include "Lang/Operators.h"

void ProberThread::init() {
	this->semaphore = xSemaphoreCreateBinary();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	this->i2cNum = checkI2CPort(hi2c);
}

bool ProberThread::probeI2C(uint8_t address) {
	return HAL_I2C_IsDeviceReady(hi2c, address << 1, 3, 100) == HAL_OK;
}

bool ProberThread::probeDB() {
	struct HX711 hx711a;
	struct HX711 hx711b;
//	hx711a = {HX711_CLK1_GPIO_Port, HX711_CLK1_Pin, HX711_DATA1_GPIO_Port, HX711_DATA1_Pin};
	hx711b = {HX711_CLK2_GPIO_Port, HX711_CLK2_Pin, HX711_DATA2_GPIO_Port, HX711_DATA2_Pin};
//	HX711_init(hx711a);
	HX711_init(hx711b);
	return HX711_checkReadiness(hx711b);
}

void ProberThread::loop() {
	if(probeI2C(BNO055_I2C_ADDR)) {
		this->instance = new IMUThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if (probeI2C(ADS_ADDR_GND)) {
		this->instance = new VoltmeterThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeDB() && hi2c == &hi2c1) {
		this->instance = instantiateHX711();
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(SEESAW_ADDRESS)) {
		this->instance = new StemmaThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x29)) {
		this->instance = new TOFThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(INA219_ADDRESS)) {
		this->instance = new INA219Thread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	HAL_I2C_DeInit(hi2c);
	HAL_I2C_Init(hi2c);
}

void ProberThread::resetProber() {
	xSemaphoreGive(semaphore);
}

Thread* ProberThread::instantiateHX711(){
		return new HX711Thread(this, HX711_CLK1_GPIO_Port, HX711_CLK1_Pin, HX711_DATA1_GPIO_Port, HX711_DATA1_Pin,
				HX711_CLK2_GPIO_Port, HX711_CLK2_Pin, HX711_DATA2_GPIO_Port, HX711_DATA2_Pin);
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
