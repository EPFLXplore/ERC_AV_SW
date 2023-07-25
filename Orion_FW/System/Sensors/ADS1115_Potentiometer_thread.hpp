/*
 * ADS1115_Potentiometer_thread.hpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent
 */

#ifndef SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_
#define SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "ADS1115.hpp"


class PotentiometerThread : public Thread {
public:
	PotentiometerThread(ProberThread* parent) : Thread("Potentiometer"), parent(parent), portNum(parent->getI2CNum()), potentiometer(parent->getI2C(), ADS_ADDR_GND) {}
	void init();
	void loop();

private:
	ProberThread* parent;
	uint8_t portNum;
	ADS1115 potentiometer;
	float MIN_VOLTAGES[4] = {1, 1, 1, 1};
	float MAX_VOLTAGES[4] = {3, 3, 3, 3};
	float MIN_ANGLES[4] = {0, 0, 0, 0}; // in deg
	float MAX_ANGLES[4] = {180, 180, 180, 180};
	float get_angle(uint8_t channel);

};

extern PotentiometerThread* PotentiometerInstance;

#endif /* SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_ */
