/*
 * ADC24.h
 *
 *  Created on: 21 Feb 2021
 *      Author: AV Team 2020
 */

#ifndef SENSORS_ADC24_H_
#define SENSORS_ADC24_H_

#include "Thread.h"
#include "DataStructures.h"

#include "Libraries/HX711/hx711.h"


class ADC24Thread : Thread {
public:
	ADC24Thread(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin);
	void init();
	void loop();
	float convertToMass(int32_t voltage);
	void calibrateMultiplier(void);
private:
	uint16_t _nSamples;
	float _multiplier;
	int32_t _zero;
};


#endif /* SENSORS_ADC24_H_ */
