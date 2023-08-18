/*
 * four_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: Vincent Nguyen
 */

// Soil Moisture & Temperature & EC & PH Sensor: RS-ECTHPH-N01-TR-1

#include <four_in_one_thread.h>
#include "Telemetry.h"

#include "Debug.h"

static char cbuf[256]; // for printf


void FourInOneThread::init() {


	// Setup query frame for Soil Moisture & Temperature & EC & PH Sensor
	query_frame.u8id = 0x01; // slave address (manually configured)
	query_frame.u8fct = MB_FC_READ_REGISTERS; // function code (3 = read)
	query_frame.u16RegAdd = 0x0000; // start address in slave
	query_frame.u16CoilsNo = 4; // number of registers to read: moisture, temperature, EC, PH
	query_frame.u16reg = ModbusDATA; // pointer to data buffer

	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY) {
		connected = false;
		LOG_ERROR("Sensor failed to query");
	} else {
		LOG_SUCCESS("Thread successfully created");
		connected = true;
	}
}

static FOURINONEData fourinone_data;

static FOURINONEPacket fourinone_packet;

//static NPK_Packet npk_packet;

void FourInOneThread::loop() {

	// Make a query for Soil Moisture & Temperature & EC & PH Sensor
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
		fourinone_data.moisture =  ModbusDATA[0] / 10.0f; // in [%]
		fourinone_data.temperature =  (int16_t)ModbusDATA[1] / 10.0f; // in [deg C] using two's complement
		fourinone_data.conductivity =  ModbusDATA[2]; // in [us/cm]
		fourinone_data.ph =  ModbusDATA[3] / 10.0f;

		fourinone_data.toArray((uint8_t*) &fourinone_packet);

		if(monitor.enter(FOURINONE_MONITOR)) {
			println("%s", fourinone_data.toString(cbuf));
		}

		MAKE_IDENTIFIABLE(fourinone_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&fourinone_packet);
		FDCAN2_network->send(&fourinone_packet);
		portYIELD();
	}
}

bool FourInOneThread::is_connected() {
	return connected;
}





