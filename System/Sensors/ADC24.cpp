/*
 * ADC24.cpp
 *
 *  Created on: 21 Feb 2021
 *      Author: AV Team 2020
 */

#include "ADC24.h"

#include "Telemetry.h"
#include "DataStructures.h"
#include "Debug/Debug.h"

static char cbuf[256];

ADC24Thread::ADC24Thread(ProberThread* parent, GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin)
: Thread("ADC24"), parent(parent) {
	HX711_set_pins(sck_gpio, sck_pin, di_gpio, di_pin);
}

void ADC24Thread::init() {
	HX711_init();

	while(!HX711_isReady()) {
		println("HX711 initialization failed");
		parent->resetProber();
	}

	println("HX711 initialized");
}

static ScienceData data;
static Science_MeasurePacket packet;
void ADC24Thread::loop() {
	if(HX711_isReady()) {
		data.mass = HX711_valueAve(nSamples);
		println("%s", data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("HX711 disconnected");
		terminate();
		parent->resetProber();
	}
}

float convertToMass(int32_t voltage){ //TODO find relationship between voltage and mass
	return 0.0;
}
