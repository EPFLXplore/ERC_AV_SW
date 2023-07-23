/*
 * Modbus_thread.cpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent
 */



#include "Modbus_thread.hpp"

#include "Telemetry.h"

ModbusThread* modbusInstance = nullptr;
static char cbuf[256]; // for printf

void ModbusThread::init() {
	modbusInstance = this;
	init_Modbus(&ModbusH);
	this->FourInOneInstance = new FourInOneThread(&ModbusH, ModbusDATA);
	osDelay(500);
	this->NPKInstance = new NPKThread(&ModbusH, ModbusDATA);
	osDelay(1000); // wait a bit for both sensors to initialize
}


void ModbusThread::loop() {
	if (!(FourInOneInstance->is_connected()) && !(NPKInstance->is_connected())) {
		modbusInstance = nullptr;
		FourInOneInstance->terminate();
		NPKInstance->terminate();
		terminate();
		parent->resetProber();
	}
	osDelay(1000);
}

void ModbusThread::init_Modbus(modbusHandler_t* ModbusH){

	  ModbusH->uModbusType = MB_MASTER;
	  ModbusH->port =  &huart2;
	  ModbusH->u8id = 0; // For master it must be 0
	  ModbusH->u16timeOut = 1000;
	  ModbusH->EN_Port1 = HAT2_P5_GPIO_Port;
	  ModbusH->EN_Pin1 = HAT2_P5_Pin;
	  ModbusH->EN_Port2 = HAT2_P3_GPIO_Port;
	  ModbusH->EN_Pin2 = HAT2_P3_Pin;
	  ModbusH->u16regs = ModbusDATA;
	  ModbusH->u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
	  ModbusH->xTypeHW = USART_HW;

	  //Initialize Modbus library
	  vTaskSuspendAll();
	  ModbusInit(ModbusH);
	  xTaskResumeAll();

	  //Start capturing traffic on serial Port
	  ModbusStart(ModbusH);
}
