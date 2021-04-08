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

ADC16Thread::ADC16Thread(ProberThread* parent)
: Thread("ADC16"), parent(parent), ads(parent->getI2C()), offset(0)
{}

void ADC16Thread::init() {
	while (!ads.begin()) {
		println("ADS1113 initialization failed");
		parent->resetProber();
	}

	println("ADS1113 initialized");
}

static PotentiometerData data;
static Handling_GripperPacket packet;
void ADC16Thread::loop() { //Should this send a voltage or radial position?
	data.voltage = ads.readADC_SingleEnded()*ads.getMultiplier() - offset; //voltage[uV]

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		println("%s", data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("ADS1113 disconnected");
		terminate();
		parent->resetProber();
	}
}

void ADC16Thread::tareVoltage(){
	offset = ads.readADC_SingleEnded()*ads.getMultiplier();
}

