/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine
 */

#ifndef SENSORS_DUMMY_THREAD_H_
#define SENSORS_DUMMY_THREAD_H_


#include <ADS1113.hpp>
#include <max11615.h>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"



class DummyThread : public Thread {
public:
	DummyThread(ProberThread* parent) :
		Thread("Dummy"), parent(parent), portNum(parent->getI2CNum()){};
	void init();
	void loop();
private:
	MAX11615 voltmeter;
	ProberThread* parent;
	uint8_t portNum;
	float dummy_member;

};

extern DummyThread* dummySensorInstance;

#endif /* SENSORS_DUMMY_THREAD_H_ */
