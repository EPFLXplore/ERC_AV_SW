/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#include <all_in_one_thread.h>
#include "Telemetry.h"
#include "main.h"

//modbusHandler_t Modbus_ALL;
//modbusHandler_t Modbus_NPK;
//uint16_t ModbusDATA[15];
//modbus_t telegram[2];
AllInOneThread* AllInOneInstance = nullptr;
uint32_t u32NotificationValue;
static char cbuf[256]; // for printf

void AllInOneThread::init() {
//	  telegram[0].u8id = 0x02; // slave address
//	  telegram[0].u8fct = (mb_functioncode_t)3; // function code (this one is registers read)
//	  //telegram[0].u16RegAdd = 0x160; // start address in slave
//	  telegram[0].u16RegAdd = 0x01E; // start address in slave
//	  telegram[0].u16CoilsNo = 3; // number of elements (coils or registers) to read
//	  telegram[0].u16reg = ModbusDATA; // pointer to a memory array in the Arduino
//
//
//	  // telegram 0: read registers
//	  telegram[1].u8id = 0x01; // slave address
//	  telegram[1].u8fct = (mb_functioncode_t)3; // function code (this one is registers write)
//	  //telegram[1].u16RegAdd = 0x160; // start address in slave
//	  telegram[1].u16RegAdd = 0x0;
//	  telegram[1].u16CoilsNo = 4; // number of elements (coils or registers) to read
//	  telegram[1].u16reg = ModbusDATA; // pointer to a memory array in the Arduino
//	  int aux;
    init_Modbus(Modbus_ALL);
//    init_Modbus(Modbus_NPK);
	AllInOneInstance = this;
	// Initialize the sensor
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static ALLINONEData allinone_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ALLINONE_Packet allinone_packet;

static NPKData npk_data;

//static NPK_Packet npk_packet;

void AllInOneThread::loop() {
//	ModbusQuery(&Modbus_ALL, telegram[1]); // make a query
//		  u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
//		  if(u32NotificationValue != ERR_OK_QUERY)
//		  {
//			//handle error
//			//  while(1);
////			  aux = 1;
//		  }
//		  osDelay(10);
	//	  ModbusDATA[0]++;
	//	  ModbusQuery(&ModbusH, telegram[0]); // make a query
	//	  u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	//	  if(u32NotificationValue)
	//	  {
	//		//handle error
	//		//  while(1);
	//		  aux =2;
	//	  }
	//	  osDelay(10);
	// Get the sensor data. Here we only read a differential value as an example
	float values[4] = {0};
	allinone_data.moist =  (float)(((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[4] << 8 | Modbus_ALL.xBufferRX.uxBuffer[5]))/10;
	allinone_data.temp =  (float)(((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[6] << 8 | Modbus_ALL.xBufferRX.uxBuffer[7]))/10;
	allinone_data.conduct =  (float)((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[8] << 8 | Modbus_ALL.xBufferRX.uxBuffer[9]);
	allinone_data.PH =  (float)((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[10] << 8 | Modbus_ALL.xBufferRX.uxBuffer[11])/10;
//	allinone_data.toArray((uint8_t*) &allinone_packet);

	// We can print it to SVW console (optional)
//	FDCAN1_network.send(&allinone_packet);
//	osDelay(10);
//
//	npk_data.nitrogen =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[4] << 8 | Modbus_ALL.xBufferRX.uxBuffer[5]));
//	npk_data.phosphorus =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[6] << 8 | Modbus_ALL.xBufferRX.uxBuffer[7]));
//	npk_data.potassium =  (float)((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[8] << 8 | Modbus_ALL.xBufferRX.uxBuffer[9]);
//	npk_data.toArray((uint8_t*) &npk_packet);
//	FDCAN1_network.send(&npk_packet);

	portYIELD();

}

void AllInOneThread::init_Modbus(modbusHandler_t ModbusH){

	  ModbusH.uModbusType = MB_MASTER;
	  ModbusH.port =  &huart2;
	  ModbusH.u8id = 0; // For master it must be 0
	  ModbusH.u16timeOut = 1000;
	  ModbusH.EN_Port1 = GPIOD;
	  ModbusH.EN_Pin1 = GPIO_PIN_4;
	  ModbusH.EN_Port2 = GPIOD;
	  ModbusH.EN_Pin2 = GPIO_PIN_14;
	  ModbusH.u16regs = ModbusDATA;
	  ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
	  ModbusH.xTypeHW = USART_HW;
	  //Initialize Modbus library
	  vTaskSuspendAll();
	  ModbusInit(&ModbusH);
	  xTaskResumeAll();
	  //Start capturing traffic on serial Port
	  ModbusStart(&ModbusH);
}






