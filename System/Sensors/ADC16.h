/*
 * ADC16.h
 *
 *  Created on: 11 Mar 2021
 *      Author: AV Team 2020
 */

#ifndef SENSORS_ADC16_H_
#define SENSORS_ADC16_H_

#include "Thread.h"
#include "DataStructures.h"

#include "Libraries/ADS1113/ads1113.h"

class ADC16Thread : Thread {
public:
	ADC16Thread(I2C_HandleTypeDef *hi2c);
	void init();
	void loop();
	void tareVoltage();
private:
	ADS1115 ads;
	float offset;
};


#endif /* SENSORS_ADC24_H_ */
