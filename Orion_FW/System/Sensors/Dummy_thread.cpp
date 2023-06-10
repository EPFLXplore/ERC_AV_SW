/*
 * Voltmeter_thread.cpp
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine Bakkali
 */

#include "Dummy_thread.h"

#include "Telemetry.h"

DummyThread* dummySensorInstance = nullptr;
static char cbuf[256]; // for printf

void DummyThread::init() {
	dummySensorInstance = this;
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
//	bool success = !MAX11615_Init(&voltmeter, (parent->getI2C()), ADDRESS_MAX11615, 3);
//
//	// If the sensor was not found or uncorrectly initialized, reset prober
//	if(!success) {
//		dummySensorInstance = nullptr;
//		terminate();
//		parent->resetProber();
//		return;
//	}
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static DummyData dummy_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static DummySystem_DummyPacket packet;

void DummyThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	uint16_t value = 1;
//	MAX11615_ADC_Read(&voltmeter, 0, &value);
	dummy_data.data = 1;
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", dummy_data.toString(cbuf));

//	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
	dummy_data.toArray((uint8_t*) &packet);
////	FDCAN1_network
	FDCAN1_network.send(&packet);
//	portYIELD();
//	} else {
//		dummySensorInstance = nullptr;
//		terminate();
//		parent->resetProber();
//	}
	osDelay(1000);
}
