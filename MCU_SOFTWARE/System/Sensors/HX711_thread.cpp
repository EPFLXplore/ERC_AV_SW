/*
 * HX711_thread.cpp
 *
 *  Created on: 21 Feb 2021
 *      Author: AV Team 2020
 */

#include "HX711_thread.h"

#include "Telemetry.h"
#include "DataStructures.h"
#include "Debug/Debug.h"

#define TIMEOUT 30*1000  //ms

const int32_t threshold = 50000;

HX711Thread* hx711Instance = nullptr;

HX711Thread::HX711Thread(ProberThread* parent, GPIO_TypeDef *sck_gpioa, uint32_t sck_pina, GPIO_TypeDef *di_gpioa, uint32_t di_pina,
               GPIO_TypeDef *sck_gpiob, uint32_t sck_pinb, GPIO_TypeDef *di_gpiob, uint32_t di_pinb)
: Thread("HX711", 1024), parent(parent), portNum(parent->getI2CNum()), _nSamples(4), _zero(8032150/2), _multiplier(1/20.f), _hx711a({sck_gpioa, sck_pina, di_gpioa, di_pina}),
  _hx711b({sck_gpiob, sck_pinb, di_gpiob, di_pinb})
{ }



void HX711Thread::init() {
	HX711_begin(_hx711a);
	HX711_begin(_hx711b);


	if(!HX711_checkReadiness(_hx711a)) {
		terminate();
		parent->resetProber();
		return;
	}

	if(!HX711_checkReadiness(_hx711b)) {
        terminate();
        parent->resetProber();
        return;
    }

	hx711Instance = this;
	//Ensure there is no weight on top of scale on startup
	//calibrateMultiplier(); //In order to manually calibrate the scale, empirical values are taken as default values already
//	println("[i2c%d] HX711 initialised", portNum);
}

static MassData data;
static avionics_massload_packet packet;
static avionics_mass_calibrate_success_packet calibrate_success_packet;
void HX711Thread::loop() {
	if(HX711_checkReadiness(_hx711a) && HX711_checkReadiness(_hx711b)) { //check sensor is still responding
		volatile float hxa = HX711_valueAve(_hx711a, _nSamples);
		volatile float hxb = HX711_valueAve(_hx711b, _nSamples);
		volatile float mass = ((HX711_valueAve(_hx711a, _nSamples) + HX711_valueAve(_hx711b, _nSamples))/2 - _zero)*_multiplier;
		data.mass = mass;
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		hx711Instance = nullptr;
		terminate();
		parent->resetProber();
	}
}

//calibrate the voltage->mass multiplier
void HX711Thread::calibrateMultiplier(void){
	//Get zero offset
//	osDelay(200);
//	tare(HX711_valueAve(_hx711a, _nSamples*2));
//	tare(HX711_valueAve(_hx711b, _nSamples*2));
	tare();
	float calibrationWeight = 500; //in g
	//start timer; will timeout after (TIMEOUT)ms, defined above^
	uint32_t startTime = HAL_GetTick();
	while((HAL_GetTick() - startTime) < TIMEOUT){ //waits until measurements change by a certain threshold and assumes new value represents 500g
		int32_t currentVal = (HX711_valueAve(_hx711a, _nSamples) + HX711_valueAve(_hx711b, _nSamples)) / 2;
		int32_t diff = currentVal - _zero;
		if(diff > threshold){
			osDelay(200); // Give the weight some time to settle
			_multiplier = 2*calibrationWeight/(HX711_valueAve(_hx711a, _nSamples)+(HX711_valueAve(_hx711b, _nSamples)) - _zero);
			calibrate_success_packet.status = true;
			network.send(&calibrate_success_packet);
			return;
		}
		osDelay(200);
	}
	hx711Instance = nullptr;
	calibrate_success_packet.status = false;
	network.send(&calibrate_success_packet);
	terminate();
	parent->resetProber();
}

void HX711Thread::tare(void){
	_zero = (HX711_valueAve(_hx711a, _nSamples*2) + HX711_valueAve(_hx711b, _nSamples*2))/2;
}
