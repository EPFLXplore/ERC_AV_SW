/*
 * INA219_thread.cpp
 *
 *  Created on: Aug 28, 2022
 *      Author: Vincent
 */

#include "Telemetry.h"
#include "INA219_thread.h"
#include "Voltmeter_extender.h"


void INA219Thread::init() {
	HAL_StatusTypeDef status = HAL_ERROR;
	status = INA219_init(&ina219, parent->getI2C());
	if(status != HAL_OK) {
//		println("[%d] BNO055 initialization failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}
}

//static ScienceData data;
//static avionics_moisture_packet packet;
void INA219Thread::loop() {
	  HAL_StatusTypeDef status = HAL_ERROR;
	  status = INA219_ReadCurrent(&ina219);
		if(status == HAL_OK) {
			if (abs(ina219.current) > motor_voltage_threshold_ma)
				stop_extender(GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_5);
			portYIELD();
		} else {
			terminate();
			parent->resetProber();
		}
}

