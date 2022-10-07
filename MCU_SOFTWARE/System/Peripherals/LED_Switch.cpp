/*
 * LED_Switch.cpp
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#include "LED_Switch.h"


void LED_ON(GPIO_TypeDef* gpio, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(gpio, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_OFF(GPIO_TypeDef* gpio, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(gpio, GPIO_Pin, GPIO_PIN_SET);
}


