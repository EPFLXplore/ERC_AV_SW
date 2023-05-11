/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine
 */

#ifndef SENSORS_DUMMY_THREAD_H_
#define SENSORS_DUMMY_THREAD_H_


#include <ADS1113.hpp>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "max11615_sens.h"
#include "max11615_sens_defs.h"


class DummyThread : public Thread {
public:
	DummyThread(ProberThread* parent) :
		Thread("Dummy"), parent(parent), portNum(parent->getI2CNum()), dummy_sensor(parent->getI2C(), ADS_ADDR_GND), dummy_member(0),
		max_sens(*(parent->getI2C()), ADDRESS_MAX11615){};
	void init();
	void loop();
private:
	Max11615_Sens max_sens;
	ADS1113 dummy_sensor;
	ProberThread* parent;
	uint8_t portNum;
	float dummy_member;

};

extern DummyThread* dummySensorInstance;

#endif /* SENSORS_DUMMY_THREAD_H_ */
