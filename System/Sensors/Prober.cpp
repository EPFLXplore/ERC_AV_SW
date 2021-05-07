/*
 * Prober.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: arion
 */

#include "Prober.h"

#include "Barometer.h"
#include "IMU.h"
#include "ADC24.h"
#include "ADC16.h"

#include "i2c.h"

#include "Lang/Operators.h"

void ProberThread::init() {
	this->semaphore = xSemaphoreCreateBinary();
	vTaskDelay(100 / portTICK_PERIOD_MS);
}

bool ProberThread::probeI2C(uint8_t address) {
	return HAL_I2C_IsDeviceReady(hi2c, address << 1, 3, 100) == HAL_OK;
}

bool ProberThread::probeDB() {
	if(strcmp(i2cNum, "i2c1") == 0)
		HX711_set_pins(GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);
	else if(strcmp(i2cNum, "i2c2") == 0)
		HX711_set_pins(GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11);
	else if(strcmp(i2cNum, "i2c3") == 0)
		HX711_set_pins(GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9);
	else if(strcmp(i2cNum, "i2c4") == 0)
		HX711_set_pins(GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15);
	else
		return false;

	HX711_init();
	return HX711_checkReadiness();
}

void ProberThread::loop() {
	if(probeI2C(0x76)) {
		println("[%s] Barometer detected", getI2CNum());
		this->instance = new BarometerThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x28)) {
		println("[%s] Accelerometer detected", getI2CNum());
		this->instance = new IMUThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x48)) {
		println("[%s] Voltmeter detected", getI2CNum());
		this->instance = new ADC16Thread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeDB()) {
		println("[%s] Mass sensor detected", getI2CNum());
		this->instance = new ADC24Thread(this, GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else {
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	HAL_I2C_DeInit(hi2c);
	HAL_I2C_Init(hi2c);
}

void ProberThread::resetProber() {
	xSemaphoreGive(semaphore);
}

char* ProberThread::checkI2CPort(I2C_HandleTypeDef* hi2c){
	if(hi2c->Instance==I2C1)
		return "i2c1";
	else if(hi2c->Instance==I2C2)
		return "i2c2";
	else if(hi2c->Instance==I2C3)
		return "i2c3";
	else if(hi2c->Instance==I2C4)
		return "i2c4";
	else
		return "unknown";
}

char* ProberThread::getI2CNum(){
	return i2cNum;
}

