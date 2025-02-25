/*
 * npk_thread.hpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_NPK_THREAD_H_
#define SENSORS_NPK_THREAD_H_

#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "main.h"

class NPKThread : public Thread {
public:
	NPKThread(modbusHandler_t* ModbusH_, uint16_t (&ModbusDATA_)[7]) :
		Thread("NPK", osPriorityHigh), ModbusH(ModbusH_), ModbusDATA(ModbusDATA_) {};
	void init();
	void loop();
	bool is_connected();
private:
	modbusHandler_t* ModbusH;
	static const uint8_t reg_offset = 4; // first 4 bytes are reserved to 4 in 1 sensor
	uint16_t (&ModbusDATA)[7];
	modbus_t query_frame;
	uint32_t u32NotificationValue;
	bool connected = false;
};

//extern NPKThread* NPKInstance;




#endif /* SENSORS_NPK_THREAD_H_ */
