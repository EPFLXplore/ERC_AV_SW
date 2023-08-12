/*
 * ADS1115_Voltmeter_thread.cpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent Nguyen
 */

#include "ADS1115_Voltmeter_thread.hpp"
#include "Telemetry.h"
VoltmeterThread* VoltmeterInstance = nullptr;
static char cbuf[256]; // for printf

void VoltmeterThread::init() {

	HAL_StatusTypeDef status = voltmeter.ADS1115_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(status != HAL_OK) {
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
	voltmeter.ADSsetGain(GAIN_ONE); // FSR = 4.096 V
	voltmeter.set_scale(1, correction_factor*divider_ratio*voltmeter.get_full_scale()/(ADS_MAX_VALUE));
}

static VoltmeterData voltmeter_data;
static VoltmeterPacket packet;

void VoltmeterThread::loop() {

	HAL_StatusTypeDef status = get_voltage(voltmeter_data.voltage);

	if(status == HAL_OK) {
		printf("%s \n", voltmeter_data.toString(cbuf));
		voltmeter_data.toArray((uint8_t*) &packet);
		FDCAN1_network->send(&packet);
		portYIELD();
	} else {
		VoltmeterInstance = nullptr;
		terminate();
		parent->resetProber();
	}
}

HAL_StatusTypeDef VoltmeterThread::get_polarity(int8_t& polarity) {
	int16_t res = 0;
	HAL_StatusTypeDef status = voltmeter.ADSreadADC_SingleEnded(0, res);
	if (status != HAL_OK) {
		polarity = 0;
		return status;
	}
	polarity = (res > polarity_threshold ? -1 : 1);
	return HAL_OK;
}

HAL_StatusTypeDef VoltmeterThread::get_voltage(float& val) {
	int8_t polarity = 0;
	HAL_StatusTypeDef status = get_polarity(polarity);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = voltmeter.get_value_conv(1, val);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val*=polarity;
	return HAL_OK;
}
