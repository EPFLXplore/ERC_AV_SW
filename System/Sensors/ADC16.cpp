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
	if(!ads.begin()) {
		println("ADS1113 initialization failed");
		terminate();
		parent->resetProber();
		return;
	}

	println("ADS1113 initialized");
}

static PotentiometerData data;
static Handling_GripperPacket packet;
void ADC16Thread::loop() { //Should this send a voltage or radial position?

	int16_t raw = ads.readADC_Differential_0_1();

	if(raw) {
		data.voltage = raw*ads.getMultiplier() - offset; //voltage[V]
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
	offset = ads.readADC_Differential_0_1()*ads.getMultiplier();
}

