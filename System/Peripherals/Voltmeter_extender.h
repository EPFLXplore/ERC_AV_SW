/*
 * Voltmeter_extender.h
 *
 *  Created on: Aug 28, 2022
 *      Author: Yassine
 */

#ifndef PERIPHERALS_VOLTMETER_EXTENDER_H_
#define PERIPHERALS_VOLTMETER_EXTENDER_H_

#include "stm32h7xx_hal.h"

#define VOLTMETER_MAX_EXTENSION_INTERVAL_MS 5000

void extend_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b);
void retract_voltmeter(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b);
void stop_extender(GPIO_TypeDef* gpio_a, uint16_t GPIO_Pin_a, GPIO_TypeDef* gpio_b, uint16_t GPIO_Pin_b, bool status);
void set_extender_status(bool status);
bool get_extender_status();


#endif /* PERIPHERALS_VOLTMETER_EXTENDER_H_ */
