/*
 * AHRS_thread.h
 *
 *  Created on: 30 Jun 2023
 *      Author: Vincent Nguyen
 */

#ifndef SENSORS_AHRS_THREAD_H_
#define SENSORS_AHRS_THREAD_H_

#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
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

	LIS3MDL* get_mag_sensor();
	BMI088* get_imu_sensor();

	static void handle_set_mag_config(uint8_t sender_id, MagConfigPacket* packet);
	static void handle_set_accel_config(uint8_t sender_id, AccelConfigPacket* packet);
	static void handle_set_gyro_config(uint8_t sender_id, GyroConfigPacket* packet);

	static void handle_imu_calib(uint8_t sender_id, ImuCalibPacket* packet);

	void start_calib_accel(uint32_t num_samples);
	void start_calib_gyro(uint32_t num_samples);

	bool mag_configured = false;
	bool accel_configured = false;
	bool gyro_configured = false;

private:
	ProberThread* parent;
	uint8_t portNum;
	void QuaternionUpdate(float ax, float ay, float az,
						  float gx, float gy, float gz,
						  float mx, float my, float mz,
						  float dt);

	EulerAngles QuaternionToEuler(Quaternion q_);

	void request_config_accel();
	void request_config_gyro();
	void request_config_mag();

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

	Vector mag; 		// in uTesla
	Vector mag_raw; 	// in uTesla
	Vector acc; 		// in m/s^2
	Vector acc_raw; 	// in m/s^2
	Vector gyro; 		// in rad/s
	Vector gyro_raw;	// in rad/s

	// Sensor fusion output

	Quaternion q;

	EulerAngles rpy;

	// Timestamps

	volatile uint32_t prev_time_us;
	volatile uint32_t curr_time_us;

	// Madgwick filter parameters

	float beta = 0.65;

	long unsigned int mag_config_time = 0;
	long unsigned int accel_config_time = 0;
	long unsigned int gyro_config_time = 0;
	long unsigned int config_req_interval = 5000;

	// Calibration parameters

	uint8_t sender_id;

	bool calibrating_accel = false;
	bool calibrating_gyro = false;

	uint32_t cnt_accel = 0;
	uint32_t cnt_gyro = 0;

	uint32_t calib_samples_accel = 1000;
	uint32_t calib_samples_gyro = 1000;

	Vector acc_avg;
	Vector gyro_avg;

	Vector acc_sum;
	Vector gyro_sum;

	void send_calib_accel();
	void send_calib_gyro();

	void set_sender_id(uint8_t sender_id);

};

extern AHRSThread* AHRSInstance;




#endif /* SENSORS_AHRS_THREAD_H_ */
