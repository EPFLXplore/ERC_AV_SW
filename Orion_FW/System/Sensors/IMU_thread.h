/*
 * IMU_thread.h
 *
 *  Created on: 30 Jun 2023
 *      Author: Leo
 */

#ifndef SENSORS_IMU_THREAD_H_
#define SENSORS_IMU_THREAD_H_



#include <bmi088_sens.hpp>
#include <lis3mdl_sens.hpp>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "time.h"
#include <vector>
#include "IMU.h"
//#include "ekf_imu.h"
#define P_INIT      (10.)
#define Q_INIT      (1e-6)
#define R_INIT_ACC  (15/10.)
#define R_INIT_MAG  (0.00015/10.)


class IMUThread : public Thread {
public:
	IMUThread(ProberThread* parent) : Thread("IMU"), parent(parent), portNum(parent->getI2CNum()),
	//dummy_sensor(parent->getI2C(), BMI08_ACCEL_I2C_ADDR_PRIMARY),
		dummy_member(0), my_imu(new IMU(parent->getI2C(),P_INIT, Q_INIT, R_INIT_ACC, R_INIT_MAG)) {}
	void init();
	void loop();
	LIS3MDL_Sens::xyz apply_calibration_transformation(const LIS3MDL_Sens::xyz& mag_data);
	Vector bnoVectorToVector(BMI088_Sens::xyz v);
	Vector bnoVectorToVector(LIS3MDL_Sens::xyz v);
	Vector bnoVectorToVector(Adafruit_LIS3MDL::xyz v);
private:

	BMI088_Sens::xyz apply_calibration_gyro(const BMI088_Sens::xyz& gyro_data);
	BMI088_Sens::xyz apply_calibration_acc(const BMI088_Sens::xyz& acc_data);
	void ekf();
/*

	ADS1113 dummy_sensor;

	BMI088_Sens* bmi_dummy_sensor;
	*/
	LIS3MDL_Sens* lis_dummy_sensor;


	ProberThread* parent;
	uint8_t portNum;
	UART_HandleTypeDef huart2;
	long int counter_uart = 0;
	float counter_float_uart = 0.0f;
	float dummy_member;
	//Quaternion q{1.0, 0.0, 0.0, 0.0};
	int frequency_counter = 1;

	float frequency;
	/*std::vector<float> mag_bias;
	std::vector<std::vector<float>> mag_transform;
	std::vector<float> gyro_bias;
	std::vector<float> acc_bias;*/

	IMU* my_imu;

	TickType_t last_timestamp = 0;
};

extern IMUThread* IMUInstance;




#endif /* SENSORS_IMU_THREAD_H_ */
