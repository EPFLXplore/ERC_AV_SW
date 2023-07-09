/*
 * ADS1234_thread.hpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine
 */

#ifndef SENSORS_ADS1234_THREAD_HPP_
#define SENSORS_ADS1234_THREAD_HPP_


#include <ADS1234.hpp>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"


class ADS1234Thread : public Thread {
public:
	ADS1234Thread(ProberThread* parent) : Thread("Dummy"), parent(parent), portNum(parent->getI2CNum()), mass_sensor(&hspi2, GPIOA, GPIO_PIN_6, GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_11, GPIOA, GPIO_PIN_12, GPIOC, GPIO_PIN_8, GPIOA, GPIO_PIN_9, GPIOC, GPIO_PIN_6, GPIOC, GPIO_PIN_7) {}
	void init();
	void loop();
private:
	ADS1234 mass_sensor;
	ProberThread* parent;
	uint8_t portNum;
	long mass_value[4] = {0};

};

extern ADS1234Thread* massSensorInstance;




#endif /* SENSORS_ADS1234_THREAD_HPP_ */
