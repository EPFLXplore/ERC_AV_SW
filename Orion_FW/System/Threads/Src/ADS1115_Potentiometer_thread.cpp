   /*
 * ADS1115_Potentiometer_thread.cpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent Nguyen
 */

#include <ADS1115_Potentiometer_thread.hpp>
#include "Telemetry.h"

#include "Debug.h"

PotentiometerThread* PotentiometerInstance = nullptr;
static char cbuf[256]; // for printf

void PotentiometerThread::init() {
	PotentiometerInstance = this;
	HAL_StatusTypeDef status = potentiometer.ADS1115_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(status != HAL_OK) {
		LOG_ERROR("Thread aborted");
		terminate();
		parent->resetProber();
		return;
	}

	LOG_SUCCESS("Thread successfully created");

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
static PotentiometerPacket pot_packet;

static PotentiometerConfigRequestPacket pot_config_packet;

void PotentiometerThread::loop() {

	// Request configuration
	if((xTaskGetTickCount()-config_time > config_req_interval) && !configured) {
		LOG_INFO("Requesting configuration...");
		config_time = xTaskGetTickCount();
		pot_config_packet.req_min_voltages = true;
		pot_config_packet.req_max_voltages = true;
		pot_config_packet.req_min_angles = true;
		pot_config_packet.req_max_angles = true;
		MAKE_IDENTIFIABLE(pot_config_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&pot_config_packet);
		FDCAN2_network->send(&pot_config_packet);
		portYIELD();
	}

	HAL_StatusTypeDef status = get_angles(potentiometer_data.angles);

	if(status == HAL_OK) {

		potentiometer_data.toArray((uint8_t*) &pot_packet);

		if(monitor.enter(POT_MONITOR)) {
			println("%s", potentiometer_data.toString(cbuf));
		}

		MAKE_IDENTIFIABLE(pot_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&pot_packet);
		FDCAN2_network->send(&pot_packet);
		portYIELD();
	} else {
		LOG_SUCCESS("Thread aborted\r\n");
		PotentiometerInstance = nullptr;
		terminate();
		parent->resetProber();
	}
}

uint8_t PotentiometerThread::getPortNum() {
	return portNum;
}

HAL_StatusTypeDef PotentiometerThread::get_angle(uint8_t channel, float& val) {
	if (channel > 3) {
		LOG_ERROR("Requested channel is not available");
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status = potentiometer.get_value_conv(channel, val);
//	HAL_StatusTypeDef status = potentiometer.read_average(channel, val, 1);
	if (status != HAL_OK)
		val = 0;
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

ADS1115* PotentiometerThread::get_sensor() {
	return &potentiometer;
}

void PotentiometerThread::set_min_voltages(float min_voltages[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MIN_VOLTAGES[i] = min_voltages[i];
}

void PotentiometerThread::set_max_voltages(float max_voltages[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MAX_VOLTAGES[i] = max_voltages[i];
}

void PotentiometerThread::set_min_angles(float min_angles[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MIN_ANGLES[i] = min_angles[i];
}

void PotentiometerThread::set_max_angles(float max_angles[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MAX_ANGLES[i] = max_angles[i];
}

const float* PotentiometerThread::get_min_voltages() const {
	return MIN_VOLTAGES;
}

const float* PotentiometerThread::get_max_voltages() const {
	return MAX_VOLTAGES;
}

const float* PotentiometerThread::get_min_angles() const {
	return MIN_ANGLES;
}

const float* PotentiometerThread::get_max_angles() const {
	return MAX_ANGLES;
}

static PotentiometerConfigResponsePacket pot_config_response_packet = {};

void PotentiometerThread::handle_set_config(uint8_t sender_id, PotentiometerConfigPacket* packet) {
	pot_config_response_packet.remote_command = packet->remote_command;
	pot_config_response_packet.set_min_voltages = packet->set_min_voltages;
	pot_config_response_packet.set_max_voltages = packet->set_max_voltages;
	pot_config_response_packet.set_min_angles = packet->set_min_angles;
	pot_config_response_packet.set_max_angles = packet->set_max_angles;
	if (PotentiometerInstance != nullptr) {
		if (packet->remote_command || !(PotentiometerInstance->configured)) {
			if (PotentiometerInstance->get_sensor() != nullptr) {
				PotentiometerInstance->configured = true;
				if (packet->set_min_voltages) {
					PotentiometerInstance->set_min_voltages(packet->min_voltages);
					PotentiometerInstance->LOG_SUCCESS("Min voltages configuration set");
				}
				if (packet->set_max_voltages) {
					PotentiometerInstance->set_max_voltages(packet->max_voltages);
					PotentiometerInstance->LOG_SUCCESS("Max voltages configuration set");
				}
				if (packet->set_min_angles) {
					PotentiometerInstance->set_min_angles(packet->min_angles);
					PotentiometerInstance->LOG_SUCCESS("Min angles configuration set");
				}
				if (packet->set_max_angles) {
					PotentiometerInstance->set_max_angles(packet->max_angles);
					PotentiometerInstance->LOG_SUCCESS("Max angles configuration set");
				}
				pot_config_response_packet.success = true;
			} else {
				pot_config_response_packet.success = false;
				PotentiometerInstance->LOG_ERROR("Potentiometer sensor member non-existent");
			}
		} else {
			pot_config_response_packet.success = false;
			PotentiometerInstance->LOG_ERROR("Configuration already requested");
		}
		const float* min_voltages = PotentiometerInstance->get_min_voltages();
		const float* max_voltages = PotentiometerInstance->get_max_voltages();
		const float* min_angles = PotentiometerInstance->get_min_angles();
		const float* max_angles = PotentiometerInstance->get_max_angles();
		for (uint8_t i = 0; i < 4; ++i) {
			pot_config_response_packet.min_voltages[i] = min_voltages[i];
			pot_config_response_packet.max_voltages[i] = max_voltages[i];
			pot_config_response_packet.min_angles[i] = min_angles[i];
			pot_config_response_packet.max_angles[i] = max_angles[i];
		}
	} else {
		pot_config_response_packet.success = false;
		console.printf_error("PotentiometerThread instance does not exist yet\r\n");
	}
	MAKE_IDENTIFIABLE(pot_config_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(&pot_config_response_packet);
	FDCAN2_network->send(&pot_config_response_packet);
}


