/*
 * Servo_thread.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_SERVO_THREAD_H_
#define SENSORS_SERVO_THREAD_H_


#include <PWMDriver.hpp>
#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "Telemetry.h"

#define LEVEL_SHIFTER_HAT_ADDR (0x46)


class ServoThread : public Thread {
public:
	ServoThread(ProberThread* parent) : Thread("Servo"), parent(parent), portNum(parent->getI2CNum()) {}
	~ServoThread();
	void init();
	void loop();

	static void handle_rotate(uint8_t sender_id, ServoPacket* packet);
	static void handle_set_config(uint8_t sender_id, ServoConfigPacket* packet);

	void set_min_duty(float min_duty[4]);
	void set_max_duty(float max_duty[4]);
	void set_min_angles(float min_angles[4]);
	void set_max_angles(float max_angles[4]);

	const float* get_min_duty() const;
	const float* get_max_duty() const;
	const float* get_min_angles() const;
	const float* get_max_angles() const;

	bool sensors_exist();

	bool configured = false;
private:
	ProberThread* parent;
	uint8_t portNum;
	PWMDriver* pwm_driver1;
	PWMDriver* pwm_driver2;
	PWMDriver* pwm_driver3;
	PWMDriver* pwm_driver4;

	uint8_t num_channels = 0;

	float MIN_ANGLES[4] = {0, 0, 0, 0};
	float MAX_ANGLES[4] = {180, 180, 180, 180};
	float MIN_DUTY[4] = {5, 5, 5, 5};
	float MAX_DUTY[4] = {10, 10, 10, 10};

	ServoPacket servo_request_packet;

	HAL_StatusTypeDef set_angle(float& angle, uint8_t ch);
	void reinit_gpios();

	long unsigned int config_time = 0;
	long unsigned int config_req_interval = 5000;
};

extern ServoThread* ServoInstance;


#endif /* SENSORS_SERVO_THREAD_H_ */
