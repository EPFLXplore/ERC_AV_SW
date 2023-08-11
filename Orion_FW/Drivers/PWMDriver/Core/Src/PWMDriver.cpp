/*
 * PWMDriver.cpp
 *
 *  Created on: Aug 11, 2023
 *      Author: Vincent Nguyen
 */

#include "PWMDriver.hpp"

PWMDriver::PWMDriver(TIM_HandleTypeDef* tim_, uint8_t channel_,
		uint16_t PWM_Pin_, GPIO_TypeDef *PWM_Port_) : tim(tim_), PWM_Pin(PWM_Pin_), PWM_Port(PWM_Port_) {

	switch (channel_) {
	case 1:
		channel = TIM_CHANNEL_1;
		break;
	case 2:
		channel = TIM_CHANNEL_2;
		break;
	case 3:
		channel = TIM_CHANNEL_3;
		break;
	case 4:
		channel = TIM_CHANNEL_4;
		break;
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (PWM_Port == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (PWM_Port == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (PWM_Port == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = PWM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	// These alternate function configuration were selected
	// w.r.t. the Hat pinouts. It might not correspond with
	// timer pins that are not the same as Orion rev4's pinout
	if (tim == &htim1)
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	else if (tim == &htim3)
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	else if (tim == &htim4)
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	else if (tim == &htim8)
		GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;

	HAL_GPIO_Init(PWM_Port, &GPIO_InitStruct);
	HAL_TIM_PWM_Start(tim, channel);
}

PWMDriver::~PWMDriver() {
	HAL_TIM_PWM_Stop(tim, channel);

	if (PWM_Port == GPIOA)
		__HAL_RCC_GPIOA_CLK_DISABLE();
	else if (PWM_Port == GPIOB)
		__HAL_RCC_GPIOB_CLK_DISABLE();
	else if (PWM_Port == GPIOC)
		__HAL_RCC_GPIOC_CLK_DISABLE();

	HAL_GPIO_DeInit(PWM_Port, PWM_Pin);
}

void PWMDriver::set_pwm(float duty_cycle, float period_ms) {
	uint32_t pulse = (uint32_t)(period*duty_cycle/100 - 1);
	switch (channel) {
	case TIM_CHANNEL_1:
		tim->Instance->CCR1 = pulse;
		break;
	case TIM_CHANNEL_2:
		tim->Instance->CCR2 = pulse;
		break;
	case TIM_CHANNEL_3:
		tim->Instance->CCR3 = pulse;
		break;
	case TIM_CHANNEL_4:
		tim->Instance->CCR4 = pulse;
		break;
	}
}
