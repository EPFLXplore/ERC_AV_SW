/*
 * ADS1115_Potentiometer_thread.cpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent
 */

#include "ADS1115_Potentiometer_thread.hpp"
#include "Telemetry.h"
PotentiometerThread* PotentiometerInstance = nullptr;
static char cbuf[256]; // for printf

void PotentiometerThread::init() {

	bool success = potentiometer.ADS1115_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
	potentiometer.ADSsetGain(GAIN_ONE); // FSR = 4.096 V

	// Setup offset and scale

	// Value -> Voltage
	// voltage = value * FSR / ADS_MAX_VALUE

	// Voltage -> Angle
	// angle = (MAX_ANGLE-MIN_ANGLE)/(MAX_VOLTAGE-MIN_VOLTAGE) * (voltage-MIN_VOLTAGE)

	// Value -> Angle
	// angle = (MAX_ANGLE-MIN_ANGLE)/(MAX_VOLTAGE-MIN_VOLTAGE) * (value * (FSR / ADS_MAX_VALUE) - MIN_VOLTAGE)

	// This can be rewritten as:
	// angle = (MAX_ANGLE-MIN_ANGLE)/(MAX_VOLTAGE-MIN_VOLTAGE)*(FSR / ADS_MAX_VALUE)*(value - MIN_VOLTAGE * (ADS_MAX_VALUE/FSR))

	// This results in
	// scale = (MAX_ANGLE-MIN_ANGLE)/(MAX_VOLTAGE-MIN_VOLTAGE)*(FSR / ADS_MAX_VALUE)
	// offset = MIN_VOLTAGE * (ADS_MAX_VALUE/FSR)

	for (uint8_t ch = 0; ch < 4; ++ch) {
		float scale = (potentiometer.get_full_scale()/ADS_MAX_VALUE)*(MAX_ANGLES[ch]-MIN_ANGLES[ch])/(MAX_VOLTAGES[ch] - MIN_VOLTAGES[ch]);
		float offset = (MIN_VOLTAGES[ch] * ADS_MAX_VALUE/potentiometer.get_full_scale());
		potentiometer.set_scale(ch, scale);
		potentiometer.set_offset(ch, offset);
	}
}

static PotentiometerData potentiometer_data;
static PotentiometerPacket packet;

void PotentiometerThread::loop() {
	potentiometer_data.angles[0] = get_angle(0);
	potentiometer_data.angles[1] = get_angle(1);
	potentiometer_data.angles[2] = get_angle(2);
	potentiometer_data.angles[3] = get_angle(3);
	// We can print it to SVW console (optional)
	printf("%s \n", potentiometer_data.toString(cbuf));

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		printf("%s \n", potentiometer_data.toString(cbuf));
		potentiometer_data.toArray((uint8_t*) &packet);
		FDCAN1_network.send(&packet);
		portYIELD();
	} else {
		PotentiometerInstance = nullptr;
		terminate();
		parent->resetProber();
	}
}

float PotentiometerThread::get_angle(uint8_t channel) {
	if (channel > 3) {
		printf("Requested channel is not available. \n");
		return 0;
	}
	return potentiometer.get_value_conv(channel);
}

