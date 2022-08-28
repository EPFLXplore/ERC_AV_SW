/*
 * Voltemeter_extender.cpp
 *
 *  Created on: Aug 28, 2022
 *      Author: Yassine
 */


#include "Voltmeter_extender.h"

void extend_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b) {
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_RESET);
}

void retract_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b) {
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_RESET);
}

void stop_extender(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b) {
	HAL_GPIO_WritePin(gpio_a, GPIO_Pin_a, GPIO_PIN_SET);
	HAL_GPIO_WritePin(gpio_b, GPIO_Pin_b, GPIO_PIN_SET);
}

