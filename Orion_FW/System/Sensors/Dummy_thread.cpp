/*
 * Voltmeter_thread.cpp
 *
 *  Created on: 10 Aug 2020
 *      Author: Yassine
 */

#include "Dummy_thread.h"

#include "Telemetry.h"

DummyThread* dummySensorInstance = nullptr;
static char cbuf[256]; // for printf

void DummyThread::init() {
	dummySensorInstance = this;
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	bool success = dummy_sensor.ADS1113_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		dummySensorInstance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
	dummy_sensor.ADSsetGain(GAIN_ONE);
}

// Declare your data with the proper data structure defined in DataStructures.h
static DummyData dummy_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static DummySystem_DummyPacket packet;

void DummyThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	dummy_data.data = dummy_sensor.ADSreadADC_Differential_0_1();
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", dummy_data.toString(cbuf));

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
		dummy_data.toArray((uint8_t*) &packet);
		UART5_network.send(&packet);
		portYIELD();
	} else {
		dummySensorInstance = nullptr;
		terminate();
		parent->resetProber();
	}
	osDelay(1000);
}
