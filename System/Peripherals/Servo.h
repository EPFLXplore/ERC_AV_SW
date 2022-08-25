/*
 * Servo.h
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#ifndef PERIPHERALS_SERVO_H_
#define PERIPHERALS_SERVO_H_

#include "tim.h"

void open_servo(TIM_HandleTypeDef* timer_handle, uint8_t channel);
void close_servo(TIM_HandleTypeDef* timer_handle, uint8_t channel);


#endif /* PERIPHERALS_SERVO_H_ */
