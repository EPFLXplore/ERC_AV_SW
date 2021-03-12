/*
 * ADC16.cpp
 *
 *  Created on: 11 Mar 2021
 *      Author: AV Team 2020
 */

#include "ADC16.h"

#include "Telemetry.h"
#include "DataStructures.h"
#include "Debug/Debug.h"

static char cbuf[256];

ADC16Thread::ADC16Thread(I2C_HandleTypeDef *hi2c)
: Thread("ADC16"), ads(hi2c), offset(0)
{}

void ADC16Thread::init() {
	while (!ads.begin()) {
			println("ADS1113 initialization failed");
		  	osDelay(500);
		}
		println("ADS1113 initialized");
}

void ADC16Thread::loop() { //Should this send a voltage or radial position?
	PotentiometerData data;
	data.voltage = ads.readADC_SingleEnded()*maxVoltage - offset;

	println("%s", data.toString(cbuf));

	Handling_GripperPacket packet;
	data.toArray((float*) &packet);
	network.send(&packet);

	osDelay(100);
}

void ADC16Thread::tareVoltage(){
	offset = ads.readADC_SingleEnded()*maxVoltage;
}

