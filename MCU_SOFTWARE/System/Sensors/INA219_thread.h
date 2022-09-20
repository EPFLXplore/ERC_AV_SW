/*
 * INA219_thread.h
 *
 *  Created on: Aug 28, 2022
 *      Author: Vincent
 */

#ifndef SENSORS_INA219_THREAD_H_
#define SENSORS_INA219_THREAD_H_

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "ina219.h"

class INA219Thread : public Thread {
public:
	INA219Thread(ProberThread* parent) : Thread("INA219"), parent(parent), portNum(parent->getI2CNum()) {}
	void init();
	void loop();

private:
	INA219_t ina219;
	ProberThread* parent;
	uint8_t portNum;
	static const uint16_t motor_voltage_threshold_ma = 300;
};


#endif /* SENSORS_INA219_THREAD_H_ */
