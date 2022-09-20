/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine
 */

#ifndef SENSORS_VOLTMETER_THREAD_H_
#define SENSORS_VOLTMETER_THREAD_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"

#include "ADS1113.h"

class VoltmeterThread : public Thread {
public:
	VoltmeterThread(ProberThread* parent) : Thread("Voltmeter"), parent(parent), portNum(parent->getI2CNum()), offset(0) {}
	void init();
	void loop();
	void tare();
private:
	ads1113_t voltmeter;
	ProberThread* parent;
	uint8_t portNum;
	float offset;

};

extern VoltmeterThread* voltmeterInstance;

#endif /* SENSORS_VOLTMETER_THREAD_H_ */
