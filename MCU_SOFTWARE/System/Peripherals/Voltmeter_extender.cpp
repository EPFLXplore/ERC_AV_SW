/*
 * Voltemeter_extender.cpp
 *
 *  Created on: Aug 28, 2022
 *      Author: Yassine
 */


#include "Voltmeter_extender.h"
#include "Telemetry.h"

static bool extender_status = false;

void extend_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b) {
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_RESET);
}

void retract_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b) {
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_SET);
}

void stop_extender(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b, bool status) {
	avionics_voltmeter_motor_status_packet motor_status;
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_SET);
	motor_status.status = status;
	network.send(&motor_status);
}

void set_extender_status(bool status) {
	extender_status = status;
}

bool get_extender_status() {
	return extender_status;
}

