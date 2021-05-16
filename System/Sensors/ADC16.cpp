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
: Thread("ADC16"), parent(parent), ads(parent->getI2C()), portNum(parent->getI2CNum()), offset(0)
{}

void ADC16Thread::init() {
	if(!ads.begin()) {
		println("[i2c%d] ADS1113 initialisation failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}

	println("[i2c%d] ADS1113 initialised", portNum);
}

static PotentiometerData data;
static Avionics_ADCPacket packet;
void ADC16Thread::loop() { //Should this send a voltage or radial position?

	int16_t raw = 0;

	if(ads.readADC_Differential_0_1(raw)) {
		data.portNum = portNum;
		data.voltage = raw*ads.getMultiplier() - offset; //voltage[V]
		println("%s", data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("[i2c%d] ADS1113 disconnected", portNum);
		terminate();
		parent->resetProber();
	}
}

void ADC16Thread::tareVoltage(){
	int16_t raw = 0;
	if(!ads.readADC_Differential_0_1(raw)){
		println("[i2c%d] ADS1113 disconnected", portNum);
		terminate();
		parent->resetProber();
		return;
	}
	println("[i2c%d] ADS1113 successfully tared", portNum);
	offset = raw*ads.getMultiplier();
}

