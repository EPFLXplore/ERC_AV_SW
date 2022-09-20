/*
 * LED_Switch.h
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#ifndef PERIPHERALS_LED_SWITCH_H_
#define PERIPHERALS_LED_SWITCH_H_

#include "gpio.h"

void LED_ON(GPIO_TypeDef* gpio, uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef* gpio, uint16_t GPIO_Pin);



#endif /* PERIPHERALS_LED_SWITCH_H_ */
