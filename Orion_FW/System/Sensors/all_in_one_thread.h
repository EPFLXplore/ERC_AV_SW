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
#include "usart.h"

class AllInOneThread : public Thread {
public:
	AllInOneThread() :
		Thread("AllInOne"){};
	void init();
	void loop();
private:
	float dummy_member;
	void init_Modbus(modbusHandler_t ModbusH);
};

extern AllInOneThread* AllInOneInstance;



#endif /* SENSORS_ALL_IN_ONE_THREAD_H_ */
