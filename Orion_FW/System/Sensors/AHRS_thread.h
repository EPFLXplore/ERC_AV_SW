/*
 * AHRS_thread.h
 *
 *  Created on: 30 Jun 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_AHRS_THREAD_H_
#define SENSORS_AHRS_THREAD_H_

//#define TRANSMIT_QUAT_FOR_PLOT

//#define PRINT_GYRO_BIAS

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "LIS3MDL.hpp"
#include "BMI088.hpp"

class AHRSThread : public Thread {
public:
	AHRSThread(ProberThread* parent) : Thread("AHRS"), parent(parent), portNum(parent->getI2CNum()),
										q({1.0f, 0.0f, 0.0f, 0.0f}), prev_time_us(0), curr_time_us(0) {}
	~AHRSThread();
	void init();
	void loop();
	uint8_t getPortNum();
private:
	ProberThread* parent;
	uint8_t portNum;
	void QuaternionUpdate(float ax, float ay, float az,
						  float gx, float gy, float gz,
						  float mx, float my, float mz,
						  float dt);

	EulerAngles QuaternionToEuler(Quaternion q_);

	// Sensor instances

	LIS3MDL* magnetometer;
	BMI088* imu;


	// Sensor configurations

	LIS3MDL::Config mag_conf = {
		LIS3MDL_ULTRAHIGHMODE,
		LIS3MDL_CONTINUOUSMODE,
		LIS3MDL_DATARATE_155_HZ,
		LIS3MDL_RANGE_4_GAUSS
	};

	BMI088::Config imu_conf = {
		ODR_800HZ_BW_140HZ,
		RANGE_12G,
		ODR_1000HZ_BW_116,
		RANGE_2000
	};

	// Sensor readings

	Vector mag; // in uTesla
	Vector acc; // in m/s^2
	Vector gyro; // in rad/s

	// Sensor fusion output

	Quaternion q;

	EulerAngles rpy;

	// Timestamps

	volatile uint32_t prev_time_us;
	volatile uint32_t curr_time_us;

	// Madgwick filter parameters

	float beta = 0.65;

};

extern AHRSThread* AHRSInstance;




#endif /* SENSORS_AHRS_THREAD_H_ */
