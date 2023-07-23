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
	this->NPKInstance = new NPKThread(&ModbusH, ModbusDATA);
//	// Initialize the sensor
////	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
//	bool success = dummy_sensor.ADS1113_init();
//	// If the sensor was not found or uncorrectly initialized, reset prober
//	if(!success) {
//		dummySensorInstance = nullptr;
//		terminate();
//		parent->resetProber();
//		return;
//	}
//
//	// Sensor related configuration after successfully connected
//	dummy_sensor.ADSsetGain(GAIN_ONE);
}


void ModbusThread::loop() {
//
//	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
//		// Send data over RoCo network
//		dummy_data.toArray((uint8_t*) &packet);
//		FDCAN1_network.send(&packet);
//		portYIELD();
//	} else {
//		dummySensorInstance = nullptr;
//		terminate();
//		parent->resetProber();
//	}
	osDelay(1000);
}

void ModbusThread::init_Modbus(modbusHandler_t* ModbusH){

	  ModbusH->uModbusType = MB_MASTER;
	  ModbusH->port =  &huart2;
	  ModbusH->u8id = 0; // For master it must be 0
	  ModbusH->u16timeOut = 1000;
	  ModbusH->EN_Port1 = GPIOD;
	  ModbusH->EN_Pin1 = GPIO_PIN_4;
	  ModbusH->EN_Port2 = GPIOD;
	  ModbusH->EN_Pin2 = GPIO_PIN_14;
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
