#ifndef PWMDRIVER_CORE_INC_PWMDRIVER_HPP_
#define PWMDRIVER_CORE_INC_PWMDRIVER_HPP_

#include "tim.h"

class PWMDriver {
public:
	PWMDriver(TIM_HandleTypeDef* tim, uint8_t channel, uint16_t PWM_Pin, GPIO_TypeDef *PWM_Port);
	~PWMDriver();
	void set_pwm(float duty_cycle, float period_ms = 20);
private:
	uint16_t channel;
	TIM_HandleTypeDef* tim;

	uint16_t PWM_Pin;
	GPIO_TypeDef *PWM_Port;

	uint32_t period = DEFAULT_TIM_PERIOD;
};

#endif
