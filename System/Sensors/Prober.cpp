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

bool ProberThread::probeDB() { //BIG PROBLEM: the pins are global variables of hx711.c; I don't know c well enough to circumvent this. IDEA: use semaphores to block mass sensor check as we know that we can only have one at any given time. Or somehow encapsulate the pins of hx711.c
	if(i2cNum == 1)
		return false; //HX711_set_pins(GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);
	else if(i2cNum == 2)
		HX711_set_pins(GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11);
	else if(i2cNum == 3)
		return false; //HX711_set_pins(GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9);
	else if(i2cNum == 4)
		return false; //HX711_set_pins(GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15);
	else
		return false;

	HX711_init();
	return HX711_checkReadiness();
}

void ProberThread::loop() {
	if(probeI2C(0x76)) {
		println("[i2c%u] Barometer detected", getI2CNum());
		this->instance = new BarometerThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x28)) {
		println("[i2c%u] Accelerometer detected", getI2CNum());
		this->instance = new IMUThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x48)) {
		println("[i2c%u] Voltmeter detected", getI2CNum());
		this->instance = new ADC16Thread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeDB()) {
		println("[i2c%u] Mass sensor detected", getI2CNum());
		this->instance = instantiateHX711();
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else {
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	//HAL_I2C_DeInit(hi2c);
	//HAL_I2C_Init(hi2c);
}

void ProberThread::resetProber() {
	xSemaphoreGive(semaphore);
}

Thread* ProberThread::instantiateHX711(){
	switch(getI2CNum()){
	case 1:
		return new ADC24Thread(this, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7); break;
	case 2:
		return new ADC24Thread(this, GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11); break;
	case 3:
		return new ADC24Thread(this, GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9); break;
	case 4:
		return new ADC24Thread(this, GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15); break;
	default:
		return NULL;
	}
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

