/*
 * Servo_thread.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: Vincent Nguyen
 */

#include <DataStructures.h>
#include <Servo_thread.h>
#include "Debug.h"
#include "stm32h7xx_hal.h"

ServoThread* ServoInstance = nullptr;

void ServoThread::init() {
	ServoInstance = this;
	switch (portNum) {
	case 1:
		num_channels = 4;
		pwm_driver1 = new PWMDriver(&htim8, 1, HAT1_P1_Pin, HAT1_P1_GPIO_Port);
		pwm_driver2 = new PWMDriver(&htim8, 2, HAT1_P2_Pin, HAT1_P2_GPIO_Port);
		pwm_driver3 = new PWMDriver(&htim8, 3, HAT1_P3_Pin, HAT1_P3_GPIO_Port);
		pwm_driver4 = new PWMDriver(&htim1, 2, HAT1_P4_Pin, HAT1_P4_GPIO_Port);
		break;
	case 2:
		num_channels = 3;
		pwm_driver1 = new PWMDriver(&htim4, 1, HAT2_P1_Pin, HAT2_P1_GPIO_Port);
		pwm_driver2 = new PWMDriver(&htim4, 2, HAT2_P2_Pin, HAT2_P2_GPIO_Port);
		pwm_driver3 = new PWMDriver(&htim4, 3, HAT2_P3_Pin, HAT2_P3_GPIO_Port);
		pwm_driver4 = nullptr;
		break;
	case 3:
		num_channels = 4;
		pwm_driver1 = new PWMDriver(&htim4, 1, HAT3_P1_Pin, HAT3_P1_GPIO_Port);
		pwm_driver2 = new PWMDriver(&htim4, 2, HAT3_P2_Pin, HAT3_P2_GPIO_Port);
		pwm_driver3 = new PWMDriver(&htim3, 3, HAT3_P3_Pin, HAT3_P3_GPIO_Port);
		pwm_driver4 = new PWMDriver(&htim3, 4, HAT3_P4_Pin, HAT3_P4_GPIO_Port);
		break;
	}
	LOG_SUCCESS("Thread successfully created \r\n");
}

static ServoData servo_data;

static ServoConfigRequestPacket servo_config_packet;

void ServoThread::loop() {
	// Request configuration
	if((xTaskGetTickCount()-config_time > config_req_interval) && !configured) {
		LOG_INFO("Requesting configuration...");
		config_time = xTaskGetTickCount();
		servo_config_packet.req_max_angles = true;
		servo_config_packet.req_min_angles = true;
		servo_config_packet.req_max_duty = true;
		servo_config_packet.req_min_duty = true;
		MAKE_IDENTIFIABLE(servo_config_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&servo_config_packet);
		FDCAN2_network->send(&servo_config_packet);
		portYIELD();
	}

	if(HAL_I2C_IsDeviceReady(parent->getI2C(), LEVEL_SHIFTER_HAT_ADDR << 1, 3, 100) == HAL_OK) {
		// maybe read from current sensor
		;
	} else {
		LOG_ERROR("Thread aborted%d \r\n");
		ServoInstance = nullptr;
		if (pwm_driver1 != nullptr) {
		delete pwm_driver1;
		pwm_driver1 = nullptr;
		}
		if (pwm_driver2 != nullptr) {
		delete pwm_driver2;
		pwm_driver2 = nullptr;
		}
		if (pwm_driver3 != nullptr) {
		delete pwm_driver3;
		pwm_driver3 = nullptr;
		}
		if (pwm_driver4 != nullptr) {
			delete pwm_driver4;
			pwm_driver4 = nullptr;
		}
		reinit_gpios();
		terminate();
		parent->resetProber();
	}
}

HAL_StatusTypeDef ServoThread::set_angle(float& angle, uint8_t ch) {

	if (ch > num_channels) {
		return HAL_ERROR;
	} else {
		if (angle > MAX_ANGLES[ch-1])
			angle = MAX_ANGLES[ch-1];
		if (angle < MIN_ANGLES[ch-1])
			angle = MIN_ANGLES[ch-1];

		float duty_cycle = MIN_DUTY[ch-1] + (angle-MIN_ANGLES[ch-1])*(MAX_DUTY[ch-1]-MIN_DUTY[ch-1])/(MAX_ANGLES[ch-1]-MIN_ANGLES[ch-1]);
		switch(ch) {
		case 1:
			pwm_driver1->set_pwm(duty_cycle);
			break;
		case 2:
			pwm_driver2->set_pwm(duty_cycle);
			break;
		case 3:
			pwm_driver3->set_pwm(duty_cycle);
			break;
		case 4:
			pwm_driver4->set_pwm(duty_cycle);
			break;
		}
		return HAL_OK;
	}
}

static ServoResponsePacket servo_response_packet;

void ServoThread::handle_rotate(uint8_t sender_id, ServoPacket* packet) {
	servo_data.success = false;
	if (ServoInstance == nullptr) {
		servo_data.success = false;
		console.printf_error("ServoThread instance does not exist yet\r\n");
		return;
	}
	else {
		float angle = packet->angle;
		HAL_StatusTypeDef status = ServoInstance->set_angle(angle, packet->channel);
		if (status == HAL_OK) {
			ServoInstance->LOG_INFO("Angle set at %f degree on channel %d", angle, packet->channel);
			servo_data.success = true;
		} else {
			servo_data.success = false;
		}
	}
	servo_data.angle = packet->angle; // normally it should have been modified by set_angle
	servo_data.channel = packet->channel;
	servo_data.toArray((uint8_t*) &servo_response_packet);
	MAKE_IDENTIFIABLE(servo_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&servo_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&servo_response_packet);
	portYIELD();
}

void ServoThread::reinit_gpios() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	switch (portNum) {
	case 1:
		GPIO_InitStruct.Pin = HAT1_P1_Pin|HAT1_P2_Pin|HAT1_P3_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(HAT1_P1_GPIO_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = HAT1_P4_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(HAT1_P4_GPIO_Port, &GPIO_InitStruct);
		break;
	case 2:
		GPIO_InitStruct.Pin = HAT2_P1_Pin|HAT2_P2_Pin|HAT2_P3_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(HAT2_P1_GPIO_Port, &GPIO_InitStruct);
		break;
	case 3:
		GPIO_InitStruct.Pin = HAT3_P1_Pin|HAT3_P2_Pin|HAT3_P3_Pin|HAT3_P4_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(HAT3_P1_GPIO_Port, &GPIO_InitStruct);
		break;
	}
}

ServoThread::~ServoThread() {
	ServoInstance = nullptr;
	if (pwm_driver1 != nullptr) {
	delete pwm_driver1;
	pwm_driver1 = nullptr;
	}
	if (pwm_driver2 != nullptr) {
	delete pwm_driver2;
	pwm_driver2 = nullptr;
	}
	if (pwm_driver3 != nullptr) {
	delete pwm_driver3;
	pwm_driver3 = nullptr;
	}
	if (pwm_driver4 != nullptr) {
		delete pwm_driver4;
		pwm_driver4 = nullptr;
	}
	reinit_gpios();
	terminate();
	parent->resetProber();
}

bool ServoThread::sensors_exist() {
	switch (num_channels) {
	case 3:
		return ((pwm_driver1 != nullptr) && (pwm_driver2 != nullptr) && (pwm_driver3 != nullptr));
	case 4:
		return ((pwm_driver1 != nullptr) && (pwm_driver2 != nullptr) && (pwm_driver3 != nullptr) && (pwm_driver4 != nullptr));
	}
	return false;
}

void ServoThread::set_min_duty(float min_duty[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MIN_DUTY[i] = min_duty[i];
}

void ServoThread::set_max_duty(float max_duty[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MAX_DUTY[i] = max_duty[i];
}

void ServoThread::set_min_angles(float min_angles[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MIN_ANGLES[i] = min_angles[i];
}

void ServoThread::set_max_angles(float max_angles[4]) {
	for (uint8_t i = 0; i < 4; ++i)
		MAX_ANGLES[i] = max_angles[i];
}

const float* ServoThread::get_min_duty() const {
	return MIN_DUTY;
}

const float* ServoThread::get_max_duty() const {
	return MAX_DUTY;
}

const float* ServoThread::get_min_angles() const {
	return MIN_ANGLES;
}

const float* ServoThread::get_max_angles() const {
	return MAX_ANGLES;
}

static ServoConfigResponsePacket servo_config_response_packet = {};

void ServoThread::handle_set_config(uint8_t sender_id, ServoConfigPacket* packet) {
	servo_config_response_packet.remote_command = packet->remote_command;
	servo_config_response_packet.set_min_duty = packet->set_min_duty;
	servo_config_response_packet.set_max_duty = packet->set_max_duty;
	servo_config_response_packet.set_min_angles = packet->set_min_angles;
	servo_config_response_packet.set_max_angles = packet->set_max_angles;
	if (ServoInstance != nullptr) {
		if (packet->remote_command || !(ServoInstance->configured)) {
			if (ServoInstance->sensors_exist()) {
				ServoInstance->configured = true;
				if (packet->set_min_duty) {
					ServoInstance->set_min_duty(packet->min_duty);
					ServoInstance->LOG_SUCCESS("Min duty configuration set");
				}
				if (packet->set_max_duty) {
					ServoInstance->set_max_duty(packet->max_duty);
					ServoInstance->LOG_SUCCESS("Max duty configuration set");
				}
				if (packet->set_min_angles) {
					ServoInstance->set_min_angles(packet->min_angles);
					ServoInstance->LOG_SUCCESS("Min angles configuration set");
				}
				if (packet->set_max_angles) {
					ServoInstance->set_max_angles(packet->max_angles);
					ServoInstance->LOG_SUCCESS("Max angles configuration set");
				}
				servo_config_response_packet.success = true;
			} else {
				servo_config_response_packet.success = false;
				ServoInstance->LOG_ERROR("Servo sensor members non-existent");
			}
		} else {
			servo_config_response_packet.success = false;
			ServoInstance->LOG_ERROR("Configuration already requested");
		}
		const float* min_duty = ServoInstance->get_min_duty();
		const float* max_duty = ServoInstance->get_max_duty();
		const float* min_angles = ServoInstance->get_min_angles();
		const float* max_angles = ServoInstance->get_max_angles();
		for (uint8_t i = 0; i < 4; ++i) {
			servo_config_response_packet.min_duty[i] = min_duty[i];
			servo_config_response_packet.max_duty[i] = max_duty[i];
			servo_config_response_packet.min_angles[i] = min_angles[i];
			servo_config_response_packet.max_angles[i] = max_angles[i];
		}
	} else {
		servo_config_response_packet.success = false;
		console.printf_error("ServoThread instance does not exist yet\r\n");
	}
	MAKE_IDENTIFIABLE(servo_config_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&servo_config_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&servo_config_response_packet);
}
