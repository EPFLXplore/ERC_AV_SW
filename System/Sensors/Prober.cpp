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
//	struct HX711 hx711a;
//	struct HX711 hx711b;
//	if(i2cNum == 1){
//		hx711a = {GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7};
//		hx711b = {GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7};
//	}
//	else if(i2cNum == 2)
//	{
//		hx711a = {GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11};
//		hx711b = {GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11};
//	}
//	else if(i2cNum == 3)
//	{
//		hx711a = {GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9};
//	    hx711b = {GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9};
//	}
//	else if(i2cNum == 4)
//	{
//		hx711a = {GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15};
//		hx711b = {GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9};
//	}
//	else
//		return false;
//	HX711_init(hx711a);
//	HX711_init(hx711b);
//	return HX711_checkReadiness(hx711a) && HX711_checkReadiness(hx711b);
}

void ProberThread::loop() {
	if(probeI2C(BNO055_I2C_ADDR)) {
//	if(true) {
//		println("[i2c%u] Accelerometer detected", getI2CNum());
		this->instance = new IMUThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(true) {
		bool status = probeI2C(ADS_ADDR_GND); // writing this in the else if statement doesn't work for some reason
		if (status) {
////		println("[i2c%u] Voltmeter detected", getI2CNum());
		this->instance = new VoltmeterThread(this);
		xSemaphoreTake(semaphore, portMAX_DELAY);}
	} else if(probeDB()) {
//		println("[i2c%u] Mass sensor detected", getI2CNum());
		this->instance = instantiateHX711();
		xSemaphoreTake(semaphore, portMAX_DELAY);
//	} else if(probeI2C(SEESAW_ADDRESS)) {
////		println("[i2c%u] Moist sensor detected", getI2CNum());
//		this->instance = new StemmaThread(this);
//		xSemaphoreTake(semaphore, portMAX_DELAY);
	} else if(probeI2C(0x52)) {
//		println("[i2c%u] TOF sensor detected", getI2CNum());
		this->instance = new TOFThread(this);
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
	switch(getI2CNum()){
	case 1:
		return new HX711Thread(this, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);
		break;
	case 2:
		return new HX711Thread(this, GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11);
		break;
	case 3:
		return new HX711Thread(this, GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9, GPIOA, GPIO_PIN_8, GPIOC, GPIO_PIN_9);
		break;
	case 4:
		return new HX711Thread(this, GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15, GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_15);
		break;
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
