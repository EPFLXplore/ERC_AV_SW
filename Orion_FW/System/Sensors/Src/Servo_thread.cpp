/*
 * Servo_thread.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: Vincent Nguyen
 */

#include "Servo_thread.h"
#include "stm32h7xx_hal.h"
#include "DataStructures.h"

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
}

// Declare your data with the proper data structure defined in DataStructures.h
static ServoData servo_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ColorFilterPacket color_filter_packet;

void ServoThread::loop() {

	if(HAL_I2C_IsDeviceReady(parent->getI2C(), LEVEL_SHIFTER_HAT_ADDR << 1, 3, 100) == HAL_OK) {
		// maybe read from current sensor
		;
	} else {
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

HAL_StatusTypeDef ServoThread::set_angle(float angle, uint8_t ch) {

	if (ch > num_channels) {
		return HAL_ERROR;
	} else {
		if (angle > MAX_ANGLE[ch-1])
			angle = MAX_ANGLE[ch-1];
		if (angle < MIN_ANGLE[ch-1])
			angle = MIN_ANGLE[ch-1];

		float duty_cycle = MIN_DUTY[ch-1] + (angle-MIN_ANGLE[ch-1])*(MAX_DUTY[ch-1]-MIN_DUTY[ch-1])/(MAX_ANGLE[ch-1]-MIN_ANGLE[ch-1]);
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
		printf("Servo instance does not exist yet. \n");
		return;
	}
	else {
		HAL_StatusTypeDef status = ServoInstance->set_angle(packet->angle, packet->channel);
		if (status == HAL_OK) {
			printf("Servo angle set at %f degree on channel %d\n", packet->angle, packet->channel);
			servo_data.success = true;
		} else {
			servo_data.success = false;
		}
	}

	servo_data.toArray((uint8_t*) &servo_response_packet);
	MAKE_IDENTIFIABLE(servo_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(&servo_response_packet);
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
