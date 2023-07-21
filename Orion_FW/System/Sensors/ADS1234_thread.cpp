/*
 * ADS1234_thread.cpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine Bakkali
 */

#include "ADS1234_thread.hpp"

#include "Telemetry.h"

ADS1234Thread* massSensorInstance = nullptr;
static char cbuf[256]; // for printf

void ADS1234Thread::init() {
	massSensorInstance = this;
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	bool success = 1;
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		massSensorInstance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
	mass_sensor.begin();
}

// Declare your data with the proper data structure defined in DataStructures.h
static MassData mass_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static MassPacket packet;

void ADS1234Thread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	mass_sensor.read(AIN3,mass_value[0],0);
	mass_data.mass = mass_value[0];
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", mass_data.toString(cbuf));

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
		mass_data.toArray((uint8_t*) &packet);
		FDCAN1_network.send(&packet);
		portYIELD();
	} else {
		massSensorInstance = nullptr;
		terminate();
		parent->resetProber();
	}
	osDelay(1000);
}



