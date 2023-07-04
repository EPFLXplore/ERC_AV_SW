/*
 * MAX11615_thread.cpp
 *
 *  Created on: 15 Jun 2023
 *      Author: YassineBakkali
 */

/*
 * MAX11615_thread.cpp
 *
 *  Created on: 15 Jun 2023
 *      Author: YassineBakkali
 */

#include "MAX11615_thread.h"

#include "Telemetry.h"

VoltmeterThread* VoltmeterInstance = nullptr;
static char cbuf[256]; // for printf

void VoltmeterThread::init() {
	VoltmeterInstance = this;
	// Initialize the sensor
	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	bool success = !MAX11615_Init(&voltmeter, (parent->getI2C()), ADDRESS_MAX11615, 3);

	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		VoltmeterInstance = nullptr;
		terminate();
		parent->resetProber();
		return;
	}
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static VoltmeterData voltmeter_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
//static VoltmeterPacket packet;

void VoltmeterThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	float value = 69;
	MAX11615_ADC_Read(&voltmeter, 0, &value);
	voltmeter_data.voltage = value;
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", voltmeter_data.toString(cbuf));

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
//		 Send data over RoCo network
//		voltmeter_data.toArray((uint8_t*) &packet);
//	FDCAN1_network.send(&packet);
	portYIELD();
	} else {
		VoltmeterInstance = nullptr;
		terminate();
		parent->resetProber();
	}
	osDelay(100);
}
