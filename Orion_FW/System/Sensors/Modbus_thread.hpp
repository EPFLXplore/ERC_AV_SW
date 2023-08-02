/*
 * Modbus_thread.hpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent
 */

#ifndef SENSORS_MODBUS_THREAD_HPP_
#define SENSORS_MODBUS_THREAD_HPP_

#define MODBUS_HAT_ADDR (0x4D) // address of I2C temperature sensor on RS485 hat

//#include <Modbus.h>
#include "Thread.h"
#include "Prober.h"
//#include "DataStructures.h"

#include "main.h" // for some reason won't build if Modbus.h is included here, we need to include main.h
//#include <Modbus.h>

#include "four_in_one_thread.h"
#include "npk_thread.h"

class ModbusThread : public Thread {
public:
	ModbusThread(ProberThread* parent) : Thread("Modbus", osPriorityHigh), parent(parent), portNum(parent->getI2CNum()){}
	void init();
	void loop();
private:
	void init_Modbus(modbusHandler_t* ModbusH);
	void reinit_gpios();
	ProberThread* parent;
	uint8_t portNum;
	modbusHandler_t ModbusH;
	uint16_t ModbusDATA[7];

	FourInOneThread* FourInOneInstance;
	NPKThread* NPKInstance;
};

extern ModbusThread* modbusInstance;


#endif /* SENSORS_MODBUS_THREAD_HPP_ */
