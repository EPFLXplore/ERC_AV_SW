/*
 * MAX11615_thread.h
 *
 *  Created on: 15 Jun 2023
 *      Author: YassineBakkali
 */

#ifndef SENSORS_MAX11615_THREAD_H_
#define SENSORS_MAX11615_THREAD_H_




#include <max11615.h>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "max11615.h"
#include "../../Drivers/ADS1115/Core/Inc/ADS1115.hpp"



class VoltmeterThread : public Thread {
public:
	VoltmeterThread(ProberThread* parent) :
		Thread("Voltmeter"), parent(parent), portNum(parent->getI2CNum()){};
	void init();
	void loop();
private:
	MAX11615 voltmeter;
	ProberThread* parent;
	uint8_t portNum;
	float dummy_member;

};

extern VoltmeterThread* VoltmeterInstance;




#endif /* SENSORS_MAX11615_THREAD_H_ */
