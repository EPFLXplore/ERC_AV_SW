/*
 * AHRS_thread.cpp
 *
 *  Created on: 30 Jun 2023
 *      Author: Leo
 */



#include <stdio.h>
#include <AHRS_thread.h>

#include "Telemetry.h"
#include <cmath>
#include <vector>
#include "tim.h"

#include "Debug.h"


AHRSThread* AHRSInstance = nullptr;
static char cbuf[256]; // for printf

AHRSThread::~AHRSThread() {
	delete magnetometer;
	magnetometer = nullptr;
}

void AHRSThread::init() {
	AHRSInstance = this;
	HAL_StatusTypeDef status;
	uint8_t err_cnt = 0;

	// Instantiate magnetometer
	magnetometer = new LIS3MDL(parent->getI2C(), mag_conf);
	status = magnetometer->init();

	if (status != HAL_OK)
		++err_cnt;

	imu = new BMI088(parent->getI2C(), imu_conf);
	status = imu->initialize_accel();

	if (status != HAL_OK)
		++err_cnt;

	status = imu->initialize_gyro();

	if (status != HAL_OK)
		++err_cnt;

	if(err_cnt != 0) {
		LOG_ERROR("Thread aborted\r\n");
		terminate();
		parent->resetProber();
		delete magnetometer;
		magnetometer = nullptr;
		delete imu;
		imu = nullptr;
		return;
	}

	LOG_SUCCESS("Thread successfully created");

#ifdef PRINT_GYRO_BIAS
	printf("Computing gyro bias (uncalibrated)... \n");
	Vector gyro_bias = {0, 0, 0};
	imu->compute_gyro_bias(gyro_bias);
	printf("Gyro bias x: %f \t y: %f \t z: %f \n", gyro_bias.x, gyro_bias.y, gyro_bias.z);
#endif

	prev_time_us = __HAL_TIM_GET_COUNTER(&htim5);
}

// Declare your data with the proper data structure defined in DataStructures.h
static IMUData imu_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static IMUPacket imu_packet;

void AHRSThread::loop() {
	HAL_StatusTypeDef status;
	uint8_t err_cnt = 0;

	status = magnetometer->get_mag_cal(mag);

	if (status != HAL_OK)
		++err_cnt;

	status = imu->get_accel_cal(acc);

	if (status != HAL_OK)
		++err_cnt;

	status = imu->get_gyro_cal(gyro);
	if (status != HAL_OK)
		++err_cnt;

	if(err_cnt == 0) {

		curr_time_us = __HAL_TIM_GET_COUNTER(&htim5);
		volatile float dt = (curr_time_us > prev_time_us) ? (curr_time_us - prev_time_us)/1000000.f : (4294967295 - prev_time_us + curr_time_us + 1)/1000000.f;
		prev_time_us = __HAL_TIM_GET_COUNTER(&htim5);
		QuaternionUpdate(acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z, mag.x, mag.y, mag.z, dt);

		// Send data over RoCo network
		imu_data.accel = acc;
		imu_data.gyro = gyro;
		imu_data.orientation = q;

		// Data monitors

		if(monitor.enter(IMU_MONITOR)) {
			println("%s", imu_data.toString(cbuf));
			monitor.exit(IMU_MONITOR);
		}

		if(monitor.enter(ACCEL_MONITOR)) {
			println("%s [m/s^2]", acc.toString(cbuf));
			monitor.exit(ACCEL_MONITOR);
		}

		if(monitor.enter(GYRO_MONITOR)) {
			println("%s [rad/s]", gyro.toString(cbuf));
			monitor.exit(GYRO_MONITOR);
		}

		if(monitor.enter(MAG_MONITOR)) {
			println("%s [uT]", mag.toString(cbuf));
			monitor.exit(MAG_MONITOR);
		}

		if(monitor.enter(QUAT_MONITOR)) {
			println("%s %.3f [Hz]", q.toString(cbuf), 1.f/dt);
			monitor.exit(QUAT_MONITOR);
		}

		if(monitor.enter(RPY_MONITOR)) {
			rpy = QuaternionToEuler(q);
			println("%s %.3f [Hz]", rpy.toString(cbuf), 1.f/dt);
			monitor.exit(RPY_MONITOR);
		}

		// Calibration monitors

		if(monitor.enter(MAG_CAL_MONITOR)) {
			Vector mag_raw = {0, 0, 0};
			magnetometer->get_mag(mag_raw);
			println("%.6f,%.6f,%.6f\n", mag_raw.x, mag_raw.y, mag_raw.z);
		}
		if(monitor.enter(ACC_CAL_MONITOR)) {
			Vector acc_raw = {0, 0, 0};
			imu->get_accel(acc_raw);
			println("%.6f,%.6f,%.6f\n", acc_raw.x, acc_raw.y, acc_raw.z);
		}

		// Plot monitors

		if(monitor.enter(IMU_PLOT_MONITOR)) {
			println("%.6f,%.6f,%.6f,%.6f\n", q.w, q.x, q.y, q.z);
		}

		imu_data.toArray((uint8_t*) &imu_packet);
		MAKE_IDENTIFIABLE(imu_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&imu_packet);
		FDCAN2_network->send(&imu_packet);
		portYIELD();
	} else {
		LOG_ERROR("Thread aborted");
		AHRSInstance = nullptr;
		terminate();
		parent->resetProber();
		delete magnetometer;
		magnetometer = nullptr;
		delete imu;
		imu = nullptr;
	}
}

uint8_t AHRSThread::getPortNum() {
	return portNum;
}

void AHRSThread::QuaternionUpdate(float ax, float ay, float az,
								 float gx, float gy, float gz,
								 float mx, float my, float mz,
								 float dt){
    float q1 = q.w, q2 = q.x, q3 = q.y, q4 = q.z;   // short name local variable for readability
    float norm;
    float hx, hy, _2bx, _2bz;
    float s1, s2, s3, s4;
    float qDot1, qDot2, qDot3, qDot4;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q1mx;
    float _2q1my;
    float _2q1mz;
    float _2q2mx;
    float _4bx;
    float _4bz;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Normalise accelerometer measurement
    norm = sqrtf(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrtf(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    mx *= norm;
    my *= norm;
    mz *= norm;

    // Reference direction of Earth's magnetic field
    _2q1mx = 2.0f * q1 * mx;
    _2q1my = 2.0f * q1 * my;
    _2q1mz = 2.0f * q1 * mz;
    _2q2mx = 2.0f * q2 * mx;
    hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
    hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
    _2bx = sqrtf(hx * hx + hy * hy);
    _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;

    // Gradient decent algorithm corrective step
    s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
    norm = 1.0f/norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;
    s4 *= norm;

    // Compute rate of change of quaternion
    qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
    qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
    qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
    qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

    // Integrate to yield quaternion
    q1 += qDot1 * dt;
    q2 += qDot2 * dt;
    q3 += qDot3 * dt;
    q4 += qDot4 * dt;
    norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q.w = q1 * norm;
    q.x = q2 * norm;
    q.y = q3 * norm;
    q.z = q4 * norm;
}

EulerAngles AHRSThread::QuaternionToEuler(Quaternion q_) {
	float roll, pitch, yaw;
	// Convert quaternions to Euler angles
	float a12 =   2.0f * (q_.x * q_.y + q_.w * q_.z);
	float a22 =   q_.w * q_.w + q_.x * q_.x - q_.y * q_.y - q_.z * q_.z;
	float a31 =   2.0f * (q_.w * q_.x + q_.y * q_.z);
	float a32 =   2.0f * (q_.x * q_.z - q_.w * q_.y);
	float a33 =   q_.w * q_.w - q_.x * q_.x - q_.y * q_.y + q_.z * q_.z;

	pitch = -asinf(a32);
	roll  = atan2f(a31, a33);
	yaw   = atan2f(a12, a22);
	pitch *= 180.0f / M_PI;
	yaw   *= 180.0f / M_PI;

	if(yaw < 0) yaw   += 360.0f; // Ensure yaw stays between 0 and 360
	roll  *= 180.0f / M_PI;

	return EulerAngles({roll, pitch, yaw});
}




