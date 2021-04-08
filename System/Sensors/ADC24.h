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
#include "Prober.h"

#include "Libraries/HX711/hx711.h"


class ADC24Thread : public Thread {
public:
	ADC24Thread(ProberThread* parent, GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin);
	void init();
	void loop();
	float convertToMass(int32_t voltage);
private:
	ProberThread* parent;
	uint16_t nSamples = 15;
};


#endif /* SENSORS_ADC24_H_ */
