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
private:
	ProberThread* parent;
	ADS1113 ads;

	float offset;
};


#endif /* SENSORS_ADC24_H_ */
