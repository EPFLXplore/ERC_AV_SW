/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

// Soil Moisture & Temperature & EC & PH Sensor: RS-ECTHPH-N01-TR-1
// Soil NPK Sensor: JXBS-3001-NPK-RS

#include <four_in_one_thread.h>
#include "Telemetry.h"
//#include "main.h"

//modbusHandler_t Modbus_ALL;
//modbusHandler_t Modbus_NPK;
//modbusHandler_t ModbusH;
//uint16_t ModbusDATA[15];


FourInOneThread* FourInOneInstance = nullptr;
static char cbuf[256]; // for printf

void FourInOneThread::init() {
//
//
//		init_Modbus(&Modbus_ALL);
	//    init_Modbus(Modbus_NPK);

//	init_Modbus(ModbusH);

	// Setup request frame for NPK sensor

//	telegram[0].u8id = 0x02; // slave address (manually configured)
//	telegram[0].u8fct = (mb_functioncode_t)3; // function code (3 = read)
//	telegram[0].u16RegAdd = 0x001E; // start address in slave
//	telegram[0].u16CoilsNo = 3; // number of registers to read: N, P, K
//	telegram[0].u16reg = ModbusDATA; // pointer to data buffer


	// Setup query frame for Soil Moisture & Temperature & EC & PH Sensor
	query_frame.u8id = 0x01; // slave address (manually configured)
	query_frame.u8fct = MB_FC_READ_REGISTERS; // function code (3 = read)
	query_frame.u16RegAdd = 0x0000; // start address in slave
	query_frame.u16CoilsNo = 4; // number of registers to read: moisture, temperature, EC, PH
	query_frame.u16reg = ModbusDATA; // pointer to data buffer
}

// Declare your data with the proper data structure defined in DataStructures.h
static FOURINONEData fourinone_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static ALLINONE_Packet allinone_packet;

//static NPK_Packet npk_packet;

void FourInOneThread::loop() {

	// Make a query for Soil Moisture & Temperature & EC & PH Sensor
	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY)
	{
	//handle error
	//  while(1);
	//			  aux = 1;
	}
	osDelay(10);

	fourinone_data.moisture =  ModbusDATA[0] / 10.0f; // in [%]
	fourinone_data.temperature =  (int16_t)ModbusDATA[1] / 10.0f; // in [deg C] using two's complement
	fourinone_data.conductivity =  ModbusDATA[2]; // in [us/cm]
	fourinone_data.ph =  ModbusDATA[3] / 10.0f;
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






