/*
 * IMU.h
 *
 *  Created on: May 27, 2023
 *      Author: leo
 */

#ifndef IMU_CORE_INC_IMU_H_
#define IMU_CORE_INC_IMU_H_

#include <bmi088_sens.hpp>
#include <lis3mdl_sens.hpp>
#include <LIS3MDL.hpp>
#include <ekf.h>
#include <matrix.h>
#include <array>
#include <Prober.h>
#include "DataStructures.h"
#include <vector>

#define IMU_ACC_Z0          (1)

class IMU{
public:
	IMU(I2C_HandleTypeDef* i2c_handle, float p_init, float rv_init, float rn_init_acc, float rn_init_mag);
    IMU(I2C_HandleTypeDef* i2c_handle);
	IMU() = delete;
    bool initialize_sensors();
    bool initialize_sensors(BMI088_Sens::Config bmi_config, LIS3MDL_Sens::Config lis_config);
    BMI088_Sens::xyz get_last_linear_accel();
    BMI088_Sens::xyz get_last_angular_accel();
//    LIS3MDL_Sens::xyz get_last_mag();
    Adafruit_LIS3MDL::xyz get_last_mag();
    Adafruit_LIS3MDL::xyz get_last_mag_cal();
    Quaternion get_last_attitude();
    void init_ekfs();


private:

    void upd_attitude();


    float P_INIT;
    float Q_INIT;
    float R_INIT_ACC;
    float R_INIT_MAG;

    static const uint8_t ss_x_len = SS_X_LEN;
    static const uint8_t ss_z_len = SS_Z_LEN;
    static const uint8_t ss_u_len = SS_U_LEN;

    //std::vector<float_prec> EKF_QINIT_data;
    float_prec EKF_PINIT_data[ss_x_len*ss_x_len];
    float_prec EKF_RINIT_data[ss_z_len*ss_z_len];
    float_prec EKF_QINIT_data[ss_x_len*ss_x_len];

//    constexpr static float_prec IMU_MAG_B0_data[3] = {cos(0), sin(0), 0.000000};

	BMI088_Sens* bmi_sensor;
//	LIS3MDL_Sens* lis_sensor;
	Adafruit_LIS3MDL* lis_sensor;
    I2C_HandleTypeDef* i2c_handle;
    BMI088_Sens::Config bmi_conf;
    LIS3MDL_Sens::Config lis_conf;

    Matrix quaternionData;
    Matrix EKF_PINIT;
    Matrix EKF_QINIT;
    Matrix EKF_RINIT;
//   static Matrix IMU_MAG_B0 = Matrix(3, 1, IMU_MAG_B0_data);


    Matrix U;
    Matrix Y;

    EKF EKF_IMU;
//    UKF UKF_IMU;

    static bool Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U, float dt);
    static bool Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U);
    static bool Main_bCalcJacobianF(Matrix& F, const Matrix& X, const Matrix& U, float dt);
    static bool Main_bCalcJacobianH(Matrix& H, const Matrix& X, const Matrix& U, float dt);


    BMI088_Sens::xyz last_acc = {0.0, 0.0, 0.0};
    BMI088_Sens::xyz last_gyr = {0.0, 0.0, 0.0};
    Adafruit_LIS3MDL::xyz last_mag = {0.0, 0.0, 0.0};
    Adafruit_LIS3MDL::xyz last_mag_cal = {0.0, 0.0, 0.0};

    Quaternion last_attitude = {0.0, 0.0, 0.0, 0.0};

    TickType_t last_timestamp = 0;
    float dt;
    

};


#endif /* IMU_CORE_INC_IMU_H_ */
