/*
 * AHRS_thread.h
 *
 *  Created on: 30 Jun 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_AHRS_THREAD_H_
#define SENSORS_AHRS_THREAD_H_

#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "LIS3MDL.hpp"
#include "BMI088.hpp"

class AHRSThread : public Thread {
public:
	AHRSThread(ProberThread* parent) : Thread("AHRS"), parent(parent), portNum(parent->getI2CNum()) {}
	~AHRSThread();
	void init();
	void loop();
private:
	ProberThread* parent;
	uint8_t portNum;
	void QuaternionUpdate(float ax, float ay, float az,
						  float gx, float gy, float gz,
						  float mx, float my, float mz,
						  float dt);

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
		ODR_1600HZ_BW_280HZ,
		RANGE_24G,
		ODR_2000HZ_BW_532,
		RANGE_2000
	};

	// Sensor readings

	Vector mag; // in uTesla
	Vector acc; // in m/s^2
	Vector gyro; // in rad/s

};

extern AHRSThread* AHRSInstance;




#endif /* SENSORS_AHRS_THREAD_H_ */
