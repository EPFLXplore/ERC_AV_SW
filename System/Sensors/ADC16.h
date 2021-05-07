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
#include "Prober.h"

#include "Libraries/ADS1113/ads1113.h"

class ADC16Thread : public Thread {
public:
	ADC16Thread(ProberThread* parent);
	void init();
	void loop();
	void tareVoltage();
	void checkPortName(I2C_HandleTypeDef* hi2c);
private:
	ProberThread* parent;
	ADS1113 ads;

	char* portNum;

	float offset;
};


#endif /* SENSORS_ADC24_H_ */
