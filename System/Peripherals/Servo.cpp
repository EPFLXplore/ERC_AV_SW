/*
 * Servo.cpp
 *
 *  Created on: Aug 25, 2022
 *      Author: Vincent
 */

#include "Servo.h"

#define DUTY_CYCLE_CLOSE_MS (1.f)
#define DUTY_CYCLE_OPEN_MS (2.f)

#define CLOSE_CCR (1000/(64000000/(htim2.Init.Prescaler*htim2.Init.Period))/DUTY_CYCLE_CLOSE_MS)
#define OPEN_CCR (1000/(64000000/(htim2.Init.Prescaler*htim2.Init.Period))/DUTY_CYCLE_OPEN_MS)


void open_servo(TIM_HandleTypeDef* timer_handle, uint8_t channel) {
	switch (channel) {
		case 1:
			timer_handle->Instance->CCR1 = OPEN_CCR;
			break;
		case 2:
			timer_handle->Instance->CCR2 = OPEN_CCR;
	}
}
void close_servo(TIM_HandleTypeDef* timer_handle, uint8_t channel) {
	switch (channel) {
		case 1:
			timer_handle->Instance->CCR1 = CLOSE_CCR;
			break;
		case 2:
			timer_handle->Instance->CCR2 = CLOSE_CCR;
	}
}



