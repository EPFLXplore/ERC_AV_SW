/*
 * all_in_one_thread.cpp
 *
 *  Created on: May 30, 2023
 *      Author: YassineBakkali
 */

#include "all_in_one_thread.h"

#include "Telemetry.h"
#include "main.h"

AllInOneThread* AllInOneInstance = nullptr;
static char cbuf[256]; // for printf

void AllInOneThread::init() {
	AllInOneInstance = this;
	// Initialize the sensor
	allSensor.initialization(HAT2_P5_Pin,  HAT2_P5_GPIO_Port,  HAT2_P3_Pin,  HAT2_P3_GPIO_Port);
	allSensor.sensor_start();
}

	// Sensor related configuration after successfully connected}

// Declare your data with the proper data structure defined in DataStructures.h
static ALLINONEData allinone_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static ALLINONE_Packet packet;

void AllInOneThread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
	float value = 0;
	value = allSensor.get_temperature();
	allinone_data.temp = value;
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", allinone_data.toString(cbuf));
	osDelay(1000);
}



