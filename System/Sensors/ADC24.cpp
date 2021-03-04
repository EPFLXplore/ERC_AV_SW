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

ADC24Thread::ADC24Thread(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin)
: Thread("ADC24"){
	HX711_set_pins(sck_gpio, sck_pin, di_gpio, di_pin);
}

void ADC24Thread::init() {
	HX711_init();
	while(!HX711_isReady()){
		println("HX711 initialization failed");
		osDelay(500);
	}
	println("HX711 initialized");
}

void ADC24Thread::loop() {
	ScienceData data;
	data.mass = HX711_valueAve(nSamples);

	println("%s", data.toString(cbuf));

	Science_MeasurePacket packet;
	data.toArray((uint32_t*) &packet);
	network.send(&packet);

	osDelay(100);
}

float convertToMass(int32_t voltage){ //TODO find relationship between voltage and mass
	return 0.0;
}
