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
	HX711_set_pins(GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11);
	HX711_init();
	return hi2c == &hi2c2 && HX711_checkReadiness();
}

void ProberThread::loop() {
	if(probeI2C(0x76)) {
		println("Barometer detected");
		this->instance = new BarometerThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x28)) {
		println("Accelerometer detected");
		this->instance = new IMUThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x48)) {
		println("Voltmeter detected");
		this->instance = new ADC16Thread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeDB()) {
		println("Mass sensor detected");
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
