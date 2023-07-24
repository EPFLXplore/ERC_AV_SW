/*
 * npk_thread.cpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent
 */

// Soil NPK Sensor: JXBS-3001-NPK-RS

#include <npk_thread.h>
#include "Telemetry.h"

static char cbuf[256]; // for printf
//NPKThread* NPKInstance = nullptr;

void NPKThread::init() {

	// Setup query frame for NPK Sensor
	query_frame.u8id = 0x02; // slave address (manually configured)
	query_frame.u8fct = MB_FC_READ_REGISTERS; // function code (3 = read)
	query_frame.u16RegAdd = 0x001E; // start address in slave
	query_frame.u16CoilsNo = 3; // number of registers to read: moisture, temperature, EC, PH
	query_frame.u16reg = ModbusDATA + reg_offset; // pointer to data buffer

//	connected = true;
	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY) {
		connected = false;
		printf("NPK Sensor failed to query \n");
	} else {
		connected = true;
	}
}


static NPKData npk_data;

static NPK_Packet npk_packet;

void NPKThread::loop() {

	// Make a query for NPK sensor
	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY)
	{
		connected = false;
		printf("NPK Sensor failed to query \n");
	}
	else
	{
	connected = true;
	npk_data.nitrogen = ModbusDATA[0 + reg_offset]; // N [mg/kg]
	npk_data.phosphorus = ModbusDATA[1 + reg_offset]; // P [mg/kg]
	npk_data.potassium = ModbusDATA[1 + reg_offset]; // K [mg/kg]

	npk_data.toArray((uint8_t*) &npk_packet);
	printf("NPK: %s \n", npk_data.toString(cbuf));
	FDCAN1_network.send(&npk_packet);
	portYIELD();
	}

	osDelay(1000);
}

bool NPKThread::is_connected() {
	return connected;
}


