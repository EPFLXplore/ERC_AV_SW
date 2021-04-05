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
const int32_t threshold = 100;

ADC24Thread::ADC24Thread(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin)
: Thread("ADC24"), _nSamples(15)
{
	HX711_set_pins(sck_gpio, sck_pin, di_gpio, di_pin);
}

void ADC24Thread::init() {
	HX711_begin();
	while(!HX711_isReady()){
		println("HX711 not responding");
		osDelay(500);
	}
	//Ensure there is no weight on top of scale on startup
	calibrateMultiplier();
	println("HX711 initialized");
}

void ADC24Thread::loop() {
	ScienceData data;
	//__disable_irq();
	data.mass = (HX711_valueAve(_nSamples) - _zero)*_multiplier;
	//__enable_irq();

	println("%s", data.toString(cbuf));

	Science_MeasurePacket packet;
	data.toArray((float*) &packet);
	network.send(&packet);

	osDelay(100);
}

float ADC24Thread::convertToMass(int32_t voltage){ //TODO find relationship between voltage and mass
	return 0.0;
}

//calibrate the voltage->mass multiplier
void ADC24Thread::calibrateMultiplier(void){
	//Put 1kg on top of scale
	_zero = HX711_valueAve(_nSamples*2);
	println("Place 1kg on top of scale in order to calibrate");
	while(true){
		int32_t currentVal = HX711_valueAve(_nSamples);
		if((currentVal - _zero) < threshold){
			_multiplier = 1.0/((HX711_valueAve(_nSamples) + currentVal)/2.0 - _zero);
			break;
		}
	}
}
