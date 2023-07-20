/*
 * NPK_thread.cpp
 *
 *  Created on: Jul 9, 2023
 *      Author: YassineBakkali
 */


#include <NPK_thread.h>
#include "Telemetry.h"
#include "main.h"

NPKThread* NPKInstance = nullptr;

void NPKThread::init() {
	NPKInstance = this;
	// Initialize the sensor
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static NPKData npk_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static NPK_Packet packet;

void NPKThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	float values[4] = {0};
	npk_data.nitrogen =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[4] << 8 | Modbus_ALL.xBufferRX.uxBuffer[5]));
	npk_data.phosphorus =  (float)(((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[6] << 8 | Modbus_ALL.xBufferRX.uxBuffer[7]));
	npk_data.potassium =  (float)((uint16_t)Modbus_NPK.xBufferRX.uxBuffer[8] << 8 | Modbus_ALL.xBufferRX.uxBuffer[9]);
	npk_data.toArray((uint8_t*) &packet);
	// We can print it to SVW console (optional)
	FDCAN1_network.send(&packet);
	portYIELD();
	osDelay(1000);
}



