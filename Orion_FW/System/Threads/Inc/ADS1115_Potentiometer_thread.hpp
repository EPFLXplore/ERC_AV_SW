/*
 * ADS1115_Potentiometer_thread.hpp
 *
 *  Created on: Jul 24, 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_
#define SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_

#define POT_CH0_ENABLE
#define POT_CH1_ENABLE
#define POT_CH2_ENABLE
#define POT_CH3_ENABLE

#include <Thread.h>
#include <Prober.h>
#include <DataStructures.h>
#include "ADS1115.hpp"


class PotentiometerThread : public Thread {
public:
	PotentiometerThread(ProberThread* parent) : Thread("Potentiometer"), parent(parent), portNum(parent->getI2CNum()), potentiometer(parent->getI2C(), ADS_ADDR_GND) {}
	void init();
	void loop();
	uint8_t getPortNum();
	ADS1115* get_sensor();

	void set_min_voltages(float min_voltages[4]);
	void set_max_voltages(float max_voltages[4]);
	void set_min_angles(float min_angles[4]);
	void set_max_angles(float max_angles[4]);

	const float* get_min_voltages() const;
	const float* get_max_voltages() const;
	const float* get_min_angles() const;
	const float* get_max_angles() const;

	static void handle_set_config(uint8_t sender_id, PotentiometerConfigPacket* packet);

	bool configured = false;
private:
	ProberThread* parent;
	uint8_t portNum;
	ADS1115 potentiometer;
	float MIN_VOLTAGES[4] = {0, 0, 0, 0};
	float MAX_VOLTAGES[4] = {3, 3, 3, 3};
	float MIN_ANGLES[4] = {0, 0, 0, 0}; // in deg
	float MAX_ANGLES[4] = {180, 180, 180, 180};
	HAL_StatusTypeDef get_angle(uint8_t channel, float& val);
	HAL_StatusTypeDef get_angles(float* angles);

	long unsigned int config_time = 0;
	long unsigned int config_req_interval = 5000;

};

extern PotentiometerThread* PotentiometerInstance;

#endif /* SENSORS_ADS1115_POTENTIOMETER_THREAD_HPP_ */
