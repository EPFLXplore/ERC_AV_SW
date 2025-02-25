/*
 * four_in_one_thread.h
 *
 *  Created on: May 30, 2023
 *      Author: Yassine Bakkali, Vincent Nguyen
 */

#ifndef SENSORS_FOUR_IN_ONE_THREAD_H_
#define SENSORS_FOUR_IN_ONE_THREAD_H_

#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "main.h"

class FourInOneThread : public Thread {
public:
	FourInOneThread(modbusHandler_t* ModbusH_, uint16_t (&ModbusDATA_)[7]) :
		Thread("FourInOne", osPriorityHigh), ModbusH(ModbusH_), ModbusDATA(ModbusDATA_) {};
	void init();
	void loop();
	bool is_connected();
private:
	modbusHandler_t* ModbusH;
	uint16_t (&ModbusDATA)[7];
	modbus_t query_frame;
	uint32_t u32NotificationValue;
	bool connected = false;
};

//extern FourInOneThread* FourInOneInstance;



#endif /* SENSORS_FOUR_IN_ONE_THREAD_H_ */
