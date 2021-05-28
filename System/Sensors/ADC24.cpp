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

#define TIMEOUT 30*1000 //ms

static char cbuf[256];
const int32_t threshold = 50000;

ADC24Thread::ADC24Thread(ProberThread* parent, GPIO_TypeDef *sck_gpio, uint32_t sck_pin, GPIO_TypeDef *di_gpio, uint32_t di_pin)
: Thread("ADC24", 1024), parent(parent), portNum(parent->getI2CNum()), _nSamples(4),
  _multiplier(1),  _zero(0), _hx711({sck_gpio, sck_pin, di_gpio, di_pin})
{ }



void ADC24Thread::init() {
	HX711_begin(_hx711);

	if(!HX711_checkReadiness(_hx711)) {
		println("[i2c%d] HX711 initialisation failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}

	//Ensure there is no weight on top of scale on startup
	calibrateMultiplier();
	println("[i2c%d] HX711 initialised", portNum);
}

static ScienceData data;
static Science_MassPacket packet;
void ADC24Thread::loop() {
	if(HX711_checkReadiness(_hx711)) { //check sensor is still responding
		data.mass = (HX711_valueAve(_hx711, _nSamples) - _zero)*_multiplier;
		println("[i2c%d] %s", portNum, data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("[i2c%d] HX711 disconnected", portNum);
		terminate();
		parent->resetProber();
	}
}

//calibrate the voltage->mass multiplier
void ADC24Thread::calibrateMultiplier(void){
	//Get zero offset
	osDelay(200);
	tare(HX711_valueAve(_hx711, _nSamples*2));
	float calibrationWeight = 500; //in g

	//start timer; will timeout after (TIMEOUT)ms, defined above^
	uint64_t start_cycle = DWT->CYCCNT;
	uint64_t cycles = TIMEOUT * (HAL_RCC_GetHCLKFreq() / 1000000L);
	while((DWT->CYCCNT - start_cycle) < cycles){ //waits until measurements change by a certain threshold and assumes new value represents 500g
		println("[i2c%d] Place %fg on top of scale in order to calibrate", portNum, calibrationWeight);
		int32_t currentVal = HX711_valueAve(_hx711, _nSamples);
		int32_t diff = currentVal - _zero;

		if(diff < -threshold){
			osDelay(200); // Give the weight some time to settle
			_multiplier = calibrationWeight/(HX711_valueAve(_hx711, _nSamples) - _zero);
			return;
		}
		osDelay(200);
	}
	println("[i2c%d] Timeout(%u): HX711 unable to be calibrated", portNum, TIMEOUT/1000);
	terminate();
	parent->resetProber();
}

void ADC24Thread::tare(int32_t zero){
	_zero = zero;
	println("[i2c%d] HX711 successfully tared", portNum);
}
