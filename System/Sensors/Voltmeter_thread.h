/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 august 2022
 *      Author: YassineBakkali
 */

#ifndef SENSORS_VOLTMETER_THREAD_H_
#define SENSORS_VOLTMETER_THREAD_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"

#include "ADS1113.h"

class VoltmeterThread : public Thread {
public:
	VoltmeterThread(ProberThread* parent) : Thread("Voltmeter"), parent(parent), portNum(parent->getI2CNum()) {}
	void init();
	void loop();

private:
//	moist_meter voltmeter;
	ads1113_t voltmeter;
	ProberThread* parent;
	uint8_t portNum;
};

#endif /* SENSORS_VOLTMETER_THREAD_H_ */
