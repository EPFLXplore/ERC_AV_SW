/*
 * IMU_thread.cpp
 *
 *  Created on: 30 Jun 2023
 *      Author: Leo
 */


#include <bmi088_sens.hpp>
#include "IMU_thread.h"
#include <stdio.h>

#include "Telemetry.h"
#include "bmi08x.h"
//#include "madgwick.hpp"
#include <chrono>
#include <cmath>
#include <vector>
#include "ekf_imu.h"


#include "matrix.h"
#include "ekf.h"
//#include "ekf.h"

/* ================================================== The AHRS/IMU variables ================================================== */
/* Gravity vector constant (align with global Z-axis) */
/* Magnetic vector constant (align with local magnetic vector) */
//class EKF;

/* ============================================ EKF variables/function declaration ============================================ */
/* Just example; in konfig.h:
 *  SS_X_LEN = 4
 *  SS_Z_LEN = 6
 *  SS_U_LEN = 3
 */
/* EKF initialization constant -------------------------------------------------------------------------------------- */

/* P(k=0) variable -------------------------------------------------------------------------------------------------- */
/*float_prec EKF_PINIT_data[SS_X_LEN*SS_X_LEN] = {P_INIT, 0,      0,      0,
                                                0,      P_INIT, 0,      0,
                                                0,      0,      P_INIT, 0,
                                                0,      0,      0,      P_INIT};
Matrix EKF_PINIT(SS_X_LEN, SS_X_LEN, EKF_PINIT_data);
 Q constant -------------------------------------------------------------------------------------------------------
float_prec EKF_QINIT_data[SS_X_LEN*SS_X_LEN] = {Q_INIT, 0,      0,      0,
                                                0,      Q_INIT, 0,      0,
                                                0,      0,      Q_INIT, 0,
                                                0,      0,      0,      Q_INIT};
Matrix EKF_QINIT(SS_X_LEN, SS_X_LEN, EKF_QINIT_data);
 R constant -------------------------------------------------------------------------------------------------------
float_prec EKF_RINIT_data[SS_Z_LEN*SS_Z_LEN] = {R_INIT_ACC, 0,          0,          0,          0,          0,
                                                0,          R_INIT_ACC, 0,          0,          0,          0,
                                                0,          0,          R_INIT_ACC, 0,          0,          0,
                                                0,          0,          0,          R_INIT_MAG, 0,          0,
                                                0,          0,          0,          0,          R_INIT_MAG, 0,
                                                0,          0,          0,          0,          0,          R_INIT_MAG};
Matrix EKF_RINIT(SS_Z_LEN, SS_Z_LEN, EKF_RINIT_data);*/
/* Nonlinear & linearization function ------------------------------------------------------------------------------- */
/*bool Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U, float dt);
bool Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U);
bool Main_bCalcJacobianF(Matrix& F, const Matrix& X, const Matrix& U, float dt);
bool Main_bCalcJacobianH(Matrix& H, const Matrix& X, const Matrix& U, float dt);*/
/* EKF variables ---------------------------------------------------------------------------------------------------- */
/*Matrix quaternionData(SS_X_LEN, 1);
Matrix Y(SS_Z_LEN, 1);
Matrix U(SS_U_LEN, 1);
 EKF system declaration -------------------------------------------------------------------------------------------
EKF EKF_IMU(quaternionData, EKF_PINIT, EKF_QINIT, EKF_RINIT,
            Main_bUpdateNonlinearX,
			Main_bUpdateNonlinearY,
			Main_bCalcJacobianF,
			Main_bCalcJacobianH);

*/


IMUThread* IMUInstance = nullptr;
static char cbuf[256]; // for printf



std::vector<float> matrixVectorMultiply(const std::vector<std::vector<float>>& A, const std::vector<float>& b) {
    int m = A.size();    // number of rows in A
    int n = A[0].size(); // number of columns in A (assumes A is non-empty)
    std::vector<float> c(m, 0.0); // initialize the result vector with zeros

    // perform matrix-vector multiplication
    for(int x = 0; x<3; x++){
    	c[x] = A[x][0] * b[0] + A[x][1]*b[1] + A[x][2]*b[2];
    }

    return c;
}

Vector quaternionToEuler(const Quaternion& q) {
    // roll (x-axis rotation)
		double yaw = 0;
		double pitch = 0;
		double roll = 0;
	    const double w2 = q.w*q.w;
	    const double x2 = q.x*q.x;
	    const double y2 = q.y*q.y;
	    const double z2 = q.z*q.z;
	    const double unitLength = w2 + x2 + y2 + z2;    // Normalised == 1, otherwise correction divisor.
	    const double abcd = q.w*q.x + q.y*q.z;
	    const double eps = 1e-7;    // TODO: pick from your math lib instead of hardcoding.
	    const double pi = 3.14159265358979323846;   // TODO: pick from your math lib instead of hardcoding.
	    if (abcd > (0.5-eps)*unitLength)
	    {
	        yaw = 2 * std::atan2(q.y, q.w);
	        pitch = pi;
	        roll = 0;
	    }
	    else if (abcd < (-0.5+eps)*unitLength)
	    {
	        yaw = -2 * std::atan2(q.y, q.w);
	        pitch = -pi;
	        roll = 0;
	    }
	    else
	    {
	        const double adbc = q.w*q.z - q.x*q.y;
	        const double acbd = q.w*q.y - q.x*q.z;
	        yaw = std::atan2(2*adbc, 1 - 2*(z2+x2));
	        pitch = std::asin(2*abcd/unitLength);
	        roll = std::atan2(2*acbd, 1 - 2*(y2+x2));
	    }
	    return {(float)yaw, (float)pitch, (float)roll};
}


void IMUThread::init() {
	IMUInstance = this;
	//his->lis_dummy_sensor = new LIS3MDL_Sens(this->parent->getI2C());
	//this->lis_dummy_sensor->init();
	bool res_init = this->my_imu->initialize_sensors();
	this->my_imu->init_ekfs();
	if(res_init == false){
		return;
	}
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	//bool success = dummy_sensor.ADS1113_init();
	// If the sensor was not found or uncorrectly initialized, reset prober
	//if(!success) {
//		dummySensorInstance = nullptr;
//		terminate();
//		parent->resetProber();
//		return;
//	}
/*	BMI088_Sens::Config conf = {
			BMI088_Sens::Odr::ODR_1600HZ_BW_280HZ,
			BMI088_Sens::Range_acc::Mg3,
			BMI088_Sens::Range_gyro::Dps_500,
			BMI088_Sens::Odr_gyro::ODR_1000HZ_BW_116,
			true,
			true,
			true
	};

	LIS3MDL_Sens::Config lis_conf = {
			LIS3MDL_Sens::LIS3MDL_Odr::Odr_1000Hz,
			LIS3MDL_Sens::LIS3MDL_Range::Range_4,
			true,
			false,
			true,
			true,
			true
	};*/


/*	this->lis_dummy_sensor = new LIS3MDL_Sens(this->parent->getI2C());
	this->lis_dummy_sensor->set_config(lis_conf);
	bool lisinit = this->lis_dummy_sensor->init();
	//this->bmi_dummy_sensor = (BMI088_Sens*)malloc(sizeof(BMI088_Sens));
	this->bmi_dummy_sensor = new BMI088_Sens(this->parent->getI2C());
	this->bmi_dummy_sensor->set_config(conf);
	bool accel_inits = this->bmi_dummy_sensor->initialize_accel();
	this->bmi_dummy_sensor->initialize_gyro();

	this->mag_bias = std::vector<float>{1.45711, 1.39509, -0.0770761};

	this->mag_transform = std::vector<std::vector<float>>
			{{0.963925, 0.0606899, -0.0142281}, {0.0606899, 0.896137, 0.00620108}, {-0.0142281, 0.00620108, 0.815979}};

	this->gyro_bias = std::vector<float>{
		0.0036176446355364457, -0.00018707559244075588, -0.0028493075692430756
	};
	this->acc_bias = std::vector<float>{
		0.0036275283943211357, -0.00010062347530493902, -0.00283879874025195
	};

	quaternionData.vSetToZero();
		quaternionData[0][0] = 1.0;
		EKF_IMU.vReset(quaternionData, EKF_PINIT, EKF_QINIT, EKF_RINIT);*/
}

LIS3MDL_Sens::xyz IMUThread::apply_calibration_transformation(const LIS3MDL_Sens::xyz& mag_data){
/*	LIS3MDL_Sens::xyz to_ret = {0.0, 0.0, 0.0};
	to_ret.x = mag_data.x - this->mag_bias[0];
	to_ret.y = mag_data.y - this->mag_bias[1];
	to_ret.z = mag_data.z - this->mag_bias[2];

	auto to_ret_vec = std::vector<float>{to_ret.x, to_ret.y, to_ret.z};

	auto mult_res = matrixVectorMultiply(this->mag_transform, to_ret_vec);
	return {mult_res[0], mult_res[1], mult_res[2]};*/
};

BMI088_Sens::xyz IMUThread::apply_calibration_acc(const BMI088_Sens::xyz& acc_data){
/*	BMI088_Sens::xyz to_ret = {0.0, 0.0, 0.0};
	to_ret.x = acc_data.x - this->acc_bias[0];
	to_ret.y = acc_data.y - this->acc_bias[1];
	to_ret.z = acc_data.z - this->acc_bias[2];

	return to_ret;*/
}

BMI088_Sens::xyz IMUThread::apply_calibration_gyro(const BMI088_Sens::xyz& gyro_data){
/*	BMI088_Sens::xyz to_ret = {0.0, 0.0, 0.0};
	to_ret.x = gyro_data.x - this->gyro_bias[0];
	to_ret.y = gyro_data.y - this->gyro_bias[1];
	to_ret.z = gyro_data.z - this->gyro_bias[2];

	return to_ret;*/
}

// Declare your data with the proper data structure defined in DataStructures.h
static IMUData imu_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static IMU_Packet packet;

void IMUThread::ekf(){
/*
	char buffer[64] = {'\0'};
	BMI088_Sens::xyz acc_ang = this->bmi_dummy_sensor->get_last_angle_accel_w_bias();
	BMI088_Sens::xyz acc = this->bmi_dummy_sensor->get_last_accel_w_bias();
	LIS3MDL_Sens::xyz mag = this->lis_dummy_sensor->get_last_mag_transformed();
	//LIS3MDL_Sens::xyz mag = this->lis_dummy_sensor->get_last_mag();

	float Ax = (-1.0)*acc.y;
	float Ay = acc.x;
	float Az = acc.z;
	float Bx = 100.0*mag.x;
	float By = 100.0*(-1.0)*mag.y;
	float Bz = 100.0*(-1.0)*mag.z;
	float p = (-1.0)*acc_ang.y;
	float q = acc_ang.x;
	float r = acc_ang.z;
	float Ax = acc.x;
	float Ay = acc.y;
	float Az = acc.z;
	float Bx = 1.0*mag.x;
	float By = 1.0*mag.y;
	float Bz = 1.0*mag.z;
	float p = acc_ang.x;
	float q = acc_ang.y;
	float r = acc_ang.z;
    U[0][0] = p;  U[1][0] = q;  U[2][0] = r;
     Output 1:3 = accelerometer
    Y[0][0] = Ax; Y[1][0] = Ay; Y[2][0] = Az;
     Output 4:6 = magnetometer
    Y[3][0] = Bx; Y[4][0] = By; Y[5][0] = Bz;

     Normalizing the output vector
    float_prec _normG = sqrt((Y[0][0] * Y[0][0]) + (Y[1][0] * Y[1][0]) + (Y[2][0] * Y[2][0]));
    Y[0][0] = Y[0][0] / _normG;
    Y[1][0] = Y[1][0] / _normG;
    Y[2][0] = Y[2][0] / _normG;
    float_prec _normM = sqrt((Y[3][0] * Y[3][0]) + (Y[4][0] * Y[4][0]) + (Y[5][0] * Y[5][0]));
    Y[3][0] = Y[3][0] / _normM;
    Y[4][0] = Y[4][0] / _normM;
    Y[5][0] = Y[5][0] / _normM;
    float_prec _normA = sqrt((U[0][0]*U[0][0]) + (U[1][0]*U[1][0]) + (U[2][0]*U[2][0]));
    U[0][0] = U[0][0] / _normA;
    U[1][0] = U[1][0] / _normA;
    U[2][0] = U[2][0] / _normA;
     ------------------ Read the sensor data / simulate the system here ------------------
    TickType_t current_timestamp = xTaskGetTickCount();
    float dt = (float)(current_timestamp - this->last_timestamp)/((float)configTICK_RATE_HZ);
    EKF_IMU.bUpdate(Y, U, dt);
    quaternionData = EKF_IMU.GetX();
    this->last_timestamp = current_timestamp;
    Quaternion quat = {quaternionData[0][0], quaternionData[1][0],
    quaternionData[2][0], quaternionData[3][0]};
    //auto vec = quaternionToEuler(quat);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",vec.x, vec.y, vec.z);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",quat.x, quat.y, quat.z);

	//printf("%.3f, %.3f, %.3f, %.3f\n", quaternionData[0][0], quaternionData[1][0], quaternionData[2][0], quaternionData[3][0]);
	sprintf(buffer, "%+.6f,%+.6f,%+.6f\n", mag.x, mag.y, mag.z);

	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, 64, 1.0);
*/


}

void IMUThread::loop() {

	char buffer[64] = {'\0'};

	//auto start_time = xTaskGetTickCount();

/*

	BMI088_Sens::xyz acc_ang = this->bmi_dummy_sensor->get_last_angle_accel_w_bias();
	//acc_ang.x *=-1.0;
	//acc_ang.y *= -1.0;
	//acc_ang.z *= -1.0;
	BMI088_Sens::xyz acc = this->bmi_dummy_sensor->get_last_accel_w_bias();
	//acc.x *= -1.0;
	//acc.y *=-1.0;
	//acc.z *= -1.0;
	//LIS3MDL_Sens::xyz mag = this->lis_dummy_sensor->get_last_mag();
	LIS3MDL_Sens::xyz mag = this->lis_dummy_sensor->get_last_mag_transformed();

*/
	//auto temp_q = madgwick_quat_upd(acc, acc_ang, mag, dt, this->q, 9.5);
	//auto vec = quaternionToEuler(temp_q);

	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n", mag.x, mag.y, mag.z);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",vec.x, vec.y, vec.z);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n", vec.x, vec.y, vec.z);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",acc_ang.x, acc_ang.y, acc_ang.z);
	//sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",acc.x, acc.y, acc.z);

	//this->ekf();

	imu_data.accel = bnoVectorToVector(this->my_imu->get_last_linear_accel());
	imu_data.gyro = bnoVectorToVector(this->my_imu->get_last_angular_accel());
	imu_data.orientation = this->my_imu->get_last_attitude();
//	imu_data.gyro = quaternionToEuler(quat);

	//Vector vec = {1.0, 12.0, 0.10};
//	sprintf(buffer, "%+.6f,%+.6f,%+.6f\n",vec.x, vec.y, vec.z);

//	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, 64, 1);
	//printf("%.3f, %.3f, %3f, %.3f\n", acc_ang.x, acc_ang.y, acc_ang.z, 0.0);

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
		imu_data.toArray((uint8_t*) &packet);
		FDCAN1_network.send(&packet);
		portYIELD();
	} else {
		IMUInstance = nullptr;
		terminate();
		parent->resetProber();
	}
	osDelay(5);
}

Vector IMUThread::bnoVectorToVector(BMI088_Sens::xyz v) {
	Vector vector;

	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;

	return vector;
}


