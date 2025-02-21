/*
 * Modbus_thread.cpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent Nguyen
 */



#include <Modbus_thread.hpp>
#include "Telemetry.h"

#include "Debug.h"

//#include "usart.h"

ModbusThread* ModbusInstance = nullptr;

ModbusThread::~ModbusThread() {
	delete FourInOneInstance;
	FourInOneInstance = nullptr;
	delete NPKInstance;
	FourInOneInstance = nullptr;
}

void ModbusThread::init() {
	ModbusInstance = this;
	MX_USART2_UART_Init();
	init_Modbus(&ModbusH);
	this->FourInOneInstance = new FourInOneThread(&ModbusH, ModbusDATA);
	this->FourInOneInstance->setTickDelay(1000);
	osDelay(500);
	this->NPKInstance = new NPKThread(&ModbusH, ModbusDATA);
	this->NPKInstance->setTickDelay(1000);
	osDelay(10000); // wait a bit for both sensors to initialize
	LOG_SUCCESS("Thread successfully created");
}


void ModbusThread::loop() {
	if ((!(FourInOneInstance->is_connected()) && !(NPKInstance->is_connected()))) {
		LOG_ERROR("NPK and 4 in 1 sensors failed. Aborting threads...");
		if (FourInOneInstance) {
			FourInOneInstance->terminate();
			delete FourInOneInstance;
			FourInOneInstance = nullptr;
		}

		if (NPKInstance) {
			NPKInstance->terminate();
			delete NPKInstance;
			NPKInstance = nullptr;
		}
		ModbusDeinit(&ModbusH);
		HAL_UART_DeInit(&huart2);
		reinit_gpios();
		terminate();
		ModbusInstance = nullptr;
		parent->resetProber();
	}
}

uint8_t ModbusThread::getPortNum() {
	return portNum;
}

bool ModbusThread::four_in_one_connected() {
	if (FourInOneInstance != nullptr)
		return FourInOneInstance->is_connected();
	else
		return false;
}

bool ModbusThread::npk_connected() {
	if (NPKInstance != nullptr)
		return NPKInstance->is_connected();
	else
		return false;
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

void ModbusThread::reinit_gpios() {
	// Reinitialize GPIO of Hat 2 that were used for UART2

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = HAT2_P4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(HAT2_P4_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = HAT2_P6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(HAT2_P6_GPIO_Port, &GPIO_InitStruct);
}
