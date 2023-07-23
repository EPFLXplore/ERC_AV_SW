/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

// Soil Moisture & Temperature & EC & PH Sensor: RS-ECTHPH-N01-TR-1
// Soil NPK Sensor: JXBS-3001-NPK-RS

#include <all_in_one_thread.h>
#include "Telemetry.h"
#include "main.h"

modbusHandler_t Modbus_ALL;
modbusHandler_t Modbus_NPK;
modbusHandler_t ModbusH;
uint16_t ModbusDATA[15];
modbus_t telegram[2];
AllInOneThread* AllInOneInstance = nullptr;
uint32_t u32NotificationValue;
static char cbuf[256]; // for printf

void AllInOneThread::init() {


		init_Modbus(&Modbus_ALL);
	//    init_Modbus(Modbus_NPK);

//	init_Modbus(ModbusH);

	// Setup request frame for NPK sensor

	telegram[0].u8id = 0x02; // slave address (manually configured)
	telegram[0].u8fct = (mb_functioncode_t)3; // function code (3 = read)
	telegram[0].u16RegAdd = 0x001E; // start address in slave
	telegram[0].u16CoilsNo = 3; // number of registers to read: N, P, K
	telegram[0].u16reg = ModbusDATA; // pointer to data buffer


	// Setup request frame for Soil Moisture & Temperature & EC & PH Sensor
	telegram[1].u8id = 0x01; // slave address (manually configured)
	telegram[1].u8fct = (mb_functioncode_t)3; // function code (3 = read)
	telegram[1].u16RegAdd = 0x0000; // start address in slave
	telegram[1].u16CoilsNo = 4; // number of registers to read: moisture, temperature, EC, PH
	telegram[1].u16reg = ModbusDATA; // pointer to data buffer
}

// Declare your data with the proper data structure defined in DataStructures.h
static ALLINONEData allinone_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ALLINONE_Packet allinone_packet;

static NPKData npk_data;

//static NPK_Packet npk_packet;

void AllInOneThread::loop() {

	// Make a query for Soil Moisture & Temperature & EC & PH Sensor
	ModbusQuery(&Modbus_ALL, telegram[1]);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY)
	{
	//handle error
	//  while(1);
	//			  aux = 1;
	}
	osDelay(10);
	ModbusDATA[0]++;
	ModbusQuery(&Modbus_ALL, telegram[1]); // make a query
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue)
	{
	//handle error
	//  while(1);
	//			  aux =2;
	}
	osDelay(10);

	uint8_t* ack_frame = Modbus_ALL.xBufferRX.uxBuffer;
	allinone_data.moist =  ((uint16_t)(ack_frame[3] << 8) | ack_frame[4]) / 10.0f; // in [%]
	allinone_data.temp =  ((int16_t)(ack_frame[5] << 8) | ack_frame[6]) / 10.0f; // in [deg C] using two's complement
	allinone_data.conduct =  (uint16_t)(ack_frame[7] << 8) | ack_frame[8]; // in [us/cm]
	allinone_data.PH =  ((uint16_t)(ack_frame[9] << 8) | ack_frame[10]) / 10.0f;

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

void AllInOneThread::init_Modbus(modbusHandler_t* ModbusH){

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

//		Modbus_ALL.uModbusType = MB_MASTER;
//		   Modbus_ALL.port =  &huart2;
//		   Modbus_ALL.u8id = 0; // For master it must be 0
//		   Modbus_ALL.u16timeOut = 1000;
//		   Modbus_ALL.EN_Port1 = GPIOD;
//		   Modbus_ALL.EN_Pin1 = GPIO_PIN_4;
//		   Modbus_ALL.EN_Port2 = GPIOD;
//		   Modbus_ALL.EN_Pin2 = GPIO_PIN_14;
//		   Modbus_ALL.u16regs = ModbusDATA;
//		   Modbus_ALL.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
//		   Modbus_ALL.xTypeHW = USART_HW;
//		   //Initialize Modbus library
//		   vTaskSuspendAll();
//		   ModbusInit(&Modbus_ALL);
//		   xTaskResumeAll();
//		   //Start capturing traffic on serial Port
//		   ModbusStart(&Modbus_ALL);
}






