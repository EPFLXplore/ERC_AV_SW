/*
 * ADS1115_Voltemeter_thread.hpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_ADS1115_VOLTMETER_THREAD_HPP_
#define SENSORS_ADS1115_VOLTMETER_THREAD_HPP_

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "ADS1115.hpp"


class VoltmeterThread : public Thread {
public:
	VoltmeterThread(ProberThread* parent) : Thread("Voltmeter"), parent(parent), portNum(parent->getI2CNum()), voltmeter(parent->getI2C(), ADS_ADDR_GND) {}
	void init();
	void loop();
	uint8_t getPortNum();
private:
	ProberThread* parent;
	uint8_t portNum;
	ADS1115 voltmeter;
	HAL_StatusTypeDef get_voltage(float& val);
	HAL_StatusTypeDef get_polarity(int8_t& polarity);
	float divider_ratio = 10.0f; // = 1/(2k / (2k + 18k))
	uint16_t polarity_threshold = 1000;
	float correction_factor = 1.0f;
};

extern VoltmeterThread* VoltmeterInstance;

#endif /* SENSORS_ADS1115_VOLTMETER_THREAD_HPP_ */
