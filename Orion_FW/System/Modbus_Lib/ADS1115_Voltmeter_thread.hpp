/*
 * ADS1115_Voltmeter_thread.hpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent
 */

#ifndef SENSORS_ADS1115_VOLTMETER_THREAD_HPP_
#define SENSORS_ADS1115_VOLTMETER_THREAD_HPP_

class DummyThread : public Thread {
public:
	DummyThread(ProberThread* parent) : Thread("Dummy"), parent(parent), portNum(parent->getI2CNum()), dummy_sensor(parent->getI2C(), ADS_ADDR_GND), dummy_member(0) {}
	void init();
	void loop();
private:
	ADS1113 dummy_sensor;
	ProberThread* parent;
	uint8_t portNum;
	float dummy_member;

};

extern DummyThread* dummySensorInstance;

#endif /* SENSORS_ADS1115_VOLTMETER_THREAD_HPP_ */
