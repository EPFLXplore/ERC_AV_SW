   /*
 * ADS1115_Potentiometer_thread.cpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent Nguyen
 */

#include "ADS1115_Potentiometer_thread.hpp"
#include "Telemetry.h"
PotentiometerThread* PotentiometerInstance = nullptr;
static char cbuf[256]; // for printf

void PotentiometerThread::init() {

	HAL_StatusTypeDef status = potentiometer.ADS1115_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(status != HAL_OK) {
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
	HAL_StatusTypeDef status = get_angles(potentiometer_data.angles);

	if(status == HAL_OK) {
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

HAL_StatusTypeDef PotentiometerThread::get_angle(uint8_t channel, float& val) {
	if (channel > 3) {
		printf("Requested channel is not available. \n");
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status = potentiometer.get_value_conv(channel, val);
//	HAL_StatusTypeDef status = potentiometer.read_average(channel, val, 1);
	if (status != HAL_OK)
		val = 0;
//	osDelay(1);
	return status;
}

HAL_StatusTypeDef PotentiometerThread::get_angles(float* angles) {
	HAL_StatusTypeDef status;
#ifdef POT_CH0_ENABLE
	status = get_angle(0, angles[0]);
	if (status != HAL_OK)
		return status;
#endif
#ifdef POT_CH1_ENABLE
	status = get_angle(1, angles[1]);
	if (status != HAL_OK)
		return status;
#endif
#ifdef POT_CH2_ENABLE
	status = get_angle(2, angles[2]);
	if (status != HAL_OK)
		return status;
#endif
#ifdef POT_CH3_ENABLE
	status = get_angle(3, angles[3]);
	if (status != HAL_OK)
		return status;
#endif

	return HAL_OK;
}

