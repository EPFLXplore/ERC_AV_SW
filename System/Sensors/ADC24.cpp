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
const int32_t threshold = 50000;

ADC24Thread::ADC24Thread(GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin)
: Thread("ADC24"), _nSamples(4), _multiplier(1),  _zero(0)
{
	HX711_set_pins(sck_gpio, sck_pin, di_gpio, di_pin);
}

void ADC24Thread::init() {
	HX711_begin();
	while(!HX711_isReady()){
		println("HX711 not responding");
		osDelay(100);
	}
	//Ensure there is no weight on top of scale on startup
	println("HX711 is responding: ensure nothing is on scale");
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

	//osDelay(10);
}

//calibrate the voltage->mass multiplier
void ADC24Thread::calibrateMultiplier(void){
	//Put 0.5kg on top of scale
	osDelay(200);
	tare(HX711_valueAve(_nSamples*2));
	while(true){
		println("Place 0.5kg on top of scale in order to calibrate");
		int32_t currentVal = HX711_valueAve(_nSamples);
		int32_t diff = currentVal - _zero;
		if(diff < -threshold){
			osDelay(200); // Give the weight some time to settle
			_multiplier = 0.5/(HX711_valueAve(_nSamples) - _zero);
			break;
		}
		osDelay(200);
	}
}

void ADC24Thread::tare(int32_t zero){
	_zero = zero;
	println("Scale successfully tared");
}
