/*
 * ADS1115_Voltmeter_thread.cpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent
 */

#include "ADS1115_Voltmeter_thread.hpp"
#include "Telemetry.h"
VoltmeterThread* VoltmeterInstance = nullptr;
static char cbuf[256]; // for printf

void VoltmeterThread::init() {

	bool success = voltmeter.ADS1115_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
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

	voltmeter_data.voltage = get_voltage();

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		printf("%s \n", voltmeter_data.toString(cbuf));
		voltmeter_data.toArray((uint8_t*) &packet);
		FDCAN1_network.send(&packet);
		portYIELD();
	} else {
		VoltmeterInstance = nullptr;
		terminate();
		parent->resetProber();
	}
	osDelay(100);
}

int8_t VoltmeterThread::get_polarity() {
	return voltmeter.ADSreadADC_SingleEnded(0) > polarity_threshold ? -1 : 1;
}

float VoltmeterThread::get_voltage() {
//	return correction_factor*get_polarity()*divider_ratio*voltmeter.get_full_scale()*
//			((float)voltmeter.ADSreadADC_SingleEnded(1)/(ADS_MAX_VALUE));
	return get_polarity()*voltmeter.get_value_conv(1);
}
