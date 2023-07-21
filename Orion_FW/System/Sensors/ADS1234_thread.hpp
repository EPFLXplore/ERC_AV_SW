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
	ADS1234Thread(ProberThread* parent, SPI_HandleTypeDef* hspi_);
	~ADS1234Thread();
	void init();
	void loop();
private:
	ProberThread* parent;
	uint8_t portNum;
	ADS1234* mass_sensor;
	float mass_value = 0;

};

extern ADS1234Thread* massSensorInstance;




#endif /* SENSORS_ADS1234_THREAD_HPP_ */
