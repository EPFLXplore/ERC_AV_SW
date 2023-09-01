/*
 * npk_thread.cpp
 *
 *  Created on: Jul 23, 2023
 *      Author: Vincent Nguyen
 */

// Soil NPK Sensor: JXBS-3001-NPK-RS

#include <npk_thread.h>
#include "Telemetry.h"

#include "Debug.h"

static char cbuf[256]; // for printf

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
		LOG_ERROR("Sensor failed to query");
	} else {
		connected = true;
		LOG_SUCCESS("Thread successfully created");
	}
}


static NPKData npk_data;

static NPKPacket npk_packet;

void NPKThread::loop() {

	// Make a query for NPK sensor
	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY)
	{
		connected = false;
		LOG_ERROR("Sensor failed to query");
	}
	else
	{
	connected = true;
	npk_data.nitrogen = ModbusDATA[0 + reg_offset]; // N [mg/kg]
	npk_data.phosphorus = ModbusDATA[1 + reg_offset]; // P [mg/kg]
	npk_data.potassium = ModbusDATA[1 + reg_offset]; // K [mg/kg]

	if(monitor.enter(NPK_MONITOR)) {
		println("%s", npk_data.toString(cbuf));
	}

	npk_data.toArray((uint8_t*) &npk_packet);

	MAKE_IDENTIFIABLE(npk_packet);
	MAKE_RELIABLE(npk_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(&npk_packet);
	FDCAN2_network->send(&npk_packet);
	portYIELD();
	}
}

bool NPKThread::is_connected() {
	return connected;
}


