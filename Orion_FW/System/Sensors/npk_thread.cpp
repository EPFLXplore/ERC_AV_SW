/*
 * npk_thread.cpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent
 */

// Soil Moisture & Temperature & EC & PH Sensor: RS-ECTHPH-N01-TR-1
// Soil NPK Sensor: JXBS-3001-NPK-RS

#include <npk_thread.h>
#include "Telemetry.h"
//#include "main.h"

//modbusHandler_t Modbus_ALL;
//modbusHandler_t Modbus_NPK;
//modbusHandler_t ModbusH;
//uint16_t ModbusDATA[15];


NPKThread* NPKInstance = nullptr;
static char cbuf[256]; // for printf

void NPKThread::init() {

	// Setup query frame for Soil Moisture & Temperature & EC & PH Sensor
	query_frame.u8id = 0x02; // slave address (manually configured)
	query_frame.u8fct = MB_FC_READ_REGISTERS; // function code (3 = read)
	query_frame.u16RegAdd = 0x001E; // start address in slave
	query_frame.u16CoilsNo = 3; // number of registers to read: moisture, temperature, EC, PH
	query_frame.u16reg = ModbusDATA + reg_offset; // pointer to data buffer
}


// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23

static NPKData npk_data;

//static NPK_Packet npk_packet;

void NPKThread::loop() {

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

	npk_data.nitrogen = ModbusDATA[0 + reg_offset]; // N [mg/kg]
	npk_data.phosphorus = ModbusDATA[1 + reg_offset]; // P [mg/kg]
	npk_data.potassium = ModbusDATA[1 + reg_offset]; // K [mg/kg]

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



