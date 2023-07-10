/*
 * all_in_one_thread.h
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#ifndef SENSORS_ALL_IN_ONE_THREAD_H_
#define SENSORS_ALL_IN_ONE_THREAD_H_

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include <ALLINONE.hpp>
#include "usart.h"

class AllInOneThread : public Thread {
public:
	AllInOneThread() :
		Thread("AllInOne"), allSensor(&huart2){};
	void init();
	void loop();
private:
	ALLINONE allSensor;
	float dummy_member;
};

extern AllInOneThread* AllInOneInstance;



#endif /* SENSORS_ALL_IN_ONE_THREAD_H_ */
