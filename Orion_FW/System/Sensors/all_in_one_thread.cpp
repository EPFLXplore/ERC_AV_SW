/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#include <all_in_one_thread.h>
#include "Telemetry.h"
#include "main.h"

AllInOneThread* AllInOneInstance = nullptr;
static char cbuf[256]; // for printf

void AllInOneThread::init() {
	AllInOneInstance = this;
	// Initialize the sensor
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static ALLINONEData allinone_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static ALLINONE_Packet allinone_packet;

static NPKData npk_data;

static NPK_Packet npk_packet;

void AllInOneThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	float values[4] = {0};
	allinone_data.moist =  (float)(((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[4] << 8 | Modbus_ALL.xBufferRX.uxBuffer[5]))/10;
	allinone_data.temp =  (float)(((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[6] << 8 | Modbus_ALL.xBufferRX.uxBuffer[7]))/10;
	allinone_data.conduct =  (float)((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[8] << 8 | Modbus_ALL.xBufferRX.uxBuffer[9]);
	allinone_data.PH =  (float)((uint16_t)Modbus_ALL.xBufferRX.uxBuffer[10] << 8 | Modbus_ALL.xBufferRX.uxBuffer[11])/10;
	allinone_data.toArray((uint8_t*) &allinone_packet);

	// We can print it to SVW console (optional)
	FDCAN1_network.send(&allinone_packet);
	osDelay(10);

	npk_data.nitrogen =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[4] << 8 | Modbus_ALL.xBufferRX.uxBuffer[5]));
	npk_data.phosphorus =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[6] << 8 | Modbus_ALL.xBufferRX.uxBuffer[7]));
	npk_data.potassium =  (float)((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[8] << 8 | Modbus_ALL.xBufferRX.uxBuffer[9]);
	npk_data.toArray((uint8_t*) &npk_packet);
	FDCAN1_network.send(&npk_packet);

	portYIELD();

}



