/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: Vincent
 */

// Soil Moisture & Temperature & EC & PH Sensor: RS-ECTHPH-N01-TR-1

#include <four_in_one_thread.h>
#include "Telemetry.h"
//#include "main.h"

static char cbuf[256]; // for printf
//FourInOneThread* FourInOneInstance = nullptr;

void FourInOneThread::init() {


	// Setup query frame for Soil Moisture & Temperature & EC & PH Sensor
	query_frame.u8id = 0x01; // slave address (manually configured)
	query_frame.u8fct = MB_FC_READ_REGISTERS; // function code (3 = read)
	query_frame.u16RegAdd = 0x0000; // start address in slave
	query_frame.u16CoilsNo = 4; // number of registers to read: moisture, temperature, EC, PH
	query_frame.u16reg = ModbusDATA; // pointer to data buffer

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

static FOURINONEData fourinone_data;

static FOURINONE_Packet fourinone_packet;

//static NPK_Packet npk_packet;

void FourInOneThread::loop() {

	// Make a query for Soil Moisture & Temperature & EC & PH Sensor
	ModbusQuery(ModbusH, query_frame);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	if(u32NotificationValue != ERR_OK_QUERY)
	{
		connected = false;
		printf("4 in 1 Sensor failed to query \n");
	}
	else
	{
		connected = true;
		fourinone_data.moisture =  ModbusDATA[0] / 10.0f; // in [%]
		fourinone_data.temperature =  (int16_t)ModbusDATA[1] / 10.0f; // in [deg C] using two's complement
		fourinone_data.conductivity =  ModbusDATA[2]; // in [us/cm]
		fourinone_data.ph =  ModbusDATA[3] / 10.0f;

		fourinone_data.toArray((uint8_t*) &fourinone_packet);
		printf("4IN1: %s \n", fourinone_data.toString(cbuf));
		FDCAN1_network.send(&fourinone_packet);
		portYIELD();
	}

	osDelay(1000);
}

bool FourInOneThread::is_connected() {
	return connected;
}





