/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine
 */

//#ifndef SENSORS_DUMMY_THREAD_H_
//#define SENSORS_DUMMY_THREAD_H_
//
//
//#include <ADS1115.hpp>
//#include "Thread.h"
//#include "Prober.h"
//#include "DataStructures.h"
//
//
//class DummyThread : public Thread {
//public:
//	DummyThread(ProberThread* parent) : Thread("Dummy"), parent(parent), portNum(parent->getI2CNum()), dummy_sensor(parent->getI2C(), ADS_ADDR_GND), dummy_member(0) {}
//	void init();
//	void loop();
//private:
//	ADS1113 dummy_sensor;
//	ProberThread* parent;
//	uint8_t portNum;
//	float dummy_member;
//
//};
//
//extern DummyThread* dummySensorInstance;
//
//#endif /* SENSORS_DUMMY_THREAD_H_ */
