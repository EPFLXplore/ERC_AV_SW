/*
 * TOF_thread.h
 *
 *  Created on: 10 august 2022
 *      Author: YassineBakkali
 */

#ifndef SENSORS_TOF_THREAD_H_
#define SENSORS_TOF_THREAD_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"

#include "vl53l1_api.h"

class TOFThread : public Thread {
public:
	TOFThread(ProberThread* parent) : Thread("TOF"), parent(parent), portNum(parent->getI2CNum()) {}
	void init();
	void loop();

private:
	VL53L1_RangingMeasurementData_t RangingData;
	VL53L1_Dev_t  vl53l1_c; // center module
	VL53L1_DEV    Dev = &vl53l1_c;
	ProberThread* parent;
	uint8_t portNum;
};

#endif /* SENSORS_VOLTMETER_THREAD_H_ */
