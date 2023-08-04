/*
 * IMU.cpp
 *
 *  Created on: May 27, 2023
 *      Author: leo
 */

#include "../Inc/IMU.h"
#include "matrix.h"

IMU::IMU(I2C_HandleTypeDef* i2c_handle):i2c_handle(i2c_handle),
    quaternionData(ss_x_len, 1),
    Q_INIT(1e-6),
    P_INIT(10.0),
    R_INIT_ACC(15/10.0),
    R_INIT_MAG(0.00015/10.0),

    EKF_QINIT_data({Q_INIT, 0,      0,      0,
                            0,      Q_INIT, 0,      0,
                            0,      0,      Q_INIT, 0,
                            0,      0,      0,      Q_INIT}),

    EKF_RINIT_data({R_INIT_ACC, 0,          0,          0,          0,          0,
                    0,          R_INIT_ACC, 0,          0,          0,          0,
                    0,          0,          R_INIT_ACC, 0,          0,          0,
                    0,          0,          0,          R_INIT_MAG, 0,          0,
                    0,          0,          0,          0,          R_INIT_MAG, 0,
                    0,          0,          0,          0,          0,          R_INIT_MAG}),

    EKF_PINIT_data({P_INIT, 0,      0,      0,
                            0,      P_INIT, 0,      0,
                            0,      0,      P_INIT, 0,
                            0,      0,      0,      P_INIT}),

    EKF_PINIT(ss_x_len, ss_x_len, EKF_PINIT_data),
    EKF_QINIT(ss_x_len, ss_x_len, EKF_QINIT_data),
    EKF_RINIT(ss_z_len, ss_z_len, EKF_RINIT_data),
    U(ss_u_len, 1),
    Y(ss_z_len, 1),
    EKF_IMU(quaternionData, EKF_PINIT, EKF_QINIT, EKF_RINIT,
    Main_bUpdateNonlinearX,
    Main_bUpdateNonlinearY,
    Main_bCalcJacobianF,
    Main_bCalcJacobianH)
    {
	    this->bmi_conf = BMI088_Sens::Config{
				BMI088_Sens::Odr::ODR_1600HZ_BW_280HZ,
				BMI088_Sens::Range_acc::Mg3,
				BMI088_Sens::Range_gyro::Dps_500,
				BMI088_Sens::Odr_gyro::ODR_1000HZ_BW_116,
				true,
				true,
				true
		};

		this->lis_conf = LIS3MDL_Sens::Config{
				LIS3MDL_Sens::LIS3MDL_Odr::Odr_1000Hz,
				LIS3MDL_Sens::LIS3MDL_Range::Range_4,
				true,
				false,
				true,
				true,
				true
		};

	};

	IMU::IMU(I2C_HandleTypeDef* i2c_handle, float p_init, float q_init, float r_init_acc, float r_init_mag): i2c_handle(i2c_handle),
    quaternionData(ss_x_len, 1), 
    Q_INIT(q_init),
    P_INIT(p_init),
    R_INIT_ACC(r_init_acc),
    R_INIT_MAG(r_init_mag),

    EKF_QINIT_data({Q_INIT, 0,      0,      0,
                            0,      Q_INIT, 0,      0,
                            0,      0,      Q_INIT, 0,
                            0,      0,      0,      Q_INIT}),

    EKF_RINIT_data({R_INIT_ACC, 0,          0,          0,          0,          0,
                    0,          R_INIT_ACC, 0,          0,          0,          0,
                    0,          0,          R_INIT_ACC, 0,          0,          0,
                    0,          0,          0,          R_INIT_MAG, 0,          0,
                    0,          0,          0,          0,          R_INIT_MAG, 0,
                    0,          0,          0,          0,          0,          R_INIT_MAG}),

    EKF_PINIT_data({P_INIT, 0,      0,      0,
                            0,      P_INIT, 0,      0,
                            0,      0,      P_INIT, 0,
                            0,      0,      0,      P_INIT}),

    EKF_PINIT(ss_x_len, ss_x_len, EKF_PINIT_data),
    EKF_QINIT(ss_x_len, ss_x_len, EKF_QINIT_data),
    EKF_RINIT(ss_z_len, ss_z_len, EKF_RINIT_data),
    U(ss_u_len, 1),
    Y(ss_z_len, 1),

    EKF_IMU(quaternionData, EKF_PINIT, EKF_QINIT, EKF_RINIT,
    Main_bUpdateNonlinearX,
    Main_bUpdateNonlinearY,
    Main_bCalcJacobianF,
    Main_bCalcJacobianH)

    {
		this->EKF_PINIT = Matrix(ss_x_len, ss_x_len, EKF_PINIT_data);
	    this->bmi_conf = BMI088_Sens::Config{
				BMI088_Sens::Odr::ODR_1600HZ_BW_280HZ,
				BMI088_Sens::Range_acc::Mg3,
				BMI088_Sens::Range_gyro::Dps_500,
				BMI088_Sens::Odr_gyro::ODR_1000HZ_BW_116,
				true,
				true,
				true
		};

		this->lis_conf = LIS3MDL_Sens::Config{
				LIS3MDL_Sens::LIS3MDL_Odr::Odr_1000Hz,
				LIS3MDL_Sens::LIS3MDL_Range::Range_4,
				true,
				false,
				true,
				true,
				true
		};
	};

bool IMU::initialize_sensors(){

    this->bmi_sensor = new BMI088_Sens(this->i2c_handle);
    this->bmi_sensor->set_config(this->bmi_conf);
    bool res_bmi_init_acc = this->bmi_sensor->initialize_accel();
    bool res_bmi_init_gyr = this->bmi_sensor->initialize_gyro();

	this->lis_sensor = new Adafruit_LIS3MDL(this->i2c_handle);
	HAL_StatusTypeDef res = lis_sensor->init();
	bool res_lis_init = (res == HAL_OK) ? true : false;
//    this->lis_sensor->set_config(this->lis_conf);
//    bool res_lis_init = this->lis_sensor->init();
	lis_sensor->setPerformanceMode(LIS3MDL_MEDIUMMODE);
	printf("Performance mode set to: \n");
	switch (lis_sensor->getPerformanceMode()) {
	    case LIS3MDL_LOWPOWERMODE: printf("Low\n"); break;
	    case LIS3MDL_MEDIUMMODE: printf("Medium\n"); break;
	    case LIS3MDL_HIGHMODE: printf("High\n"); break;
	    case LIS3MDL_ULTRAHIGHMODE: printf("Ultra-High\n"); break;
	  }
	lis_sensor->setOperationMode(LIS3MDL_CONTINUOUSMODE);
	printf("Operation mode set to: \n");
	switch (lis_sensor->getOperationMode()) {
		case LIS3MDL_CONTINUOUSMODE: printf("Continuous\n"); break;
		case LIS3MDL_SINGLEMODE: printf("Single mode\n"); break;
		case LIS3MDL_POWERDOWNMODE: printf("Power-down\n"); break;
	}
	lis_sensor->setDataRate(LIS3MDL_DATARATE_155_HZ);
	printf("Data rate set to: \n");
	switch (lis_sensor->getDataRate()) {
		case LIS3MDL_DATARATE_0_625_HZ: printf("0.625 Hz\n"); break;
		case LIS3MDL_DATARATE_1_25_HZ: printf("1.25 Hz\n"); break;
		case LIS3MDL_DATARATE_2_5_HZ: printf("2.5 Hz\n"); break;
		case LIS3MDL_DATARATE_5_HZ: printf("5 Hz\n"); break;
		case LIS3MDL_DATARATE_10_HZ: printf("10 Hz\n"); break;
		case LIS3MDL_DATARATE_20_HZ: printf("20 Hz\n"); break;
		case LIS3MDL_DATARATE_40_HZ: printf("40 Hz\n"); break;
		case LIS3MDL_DATARATE_80_HZ: printf("80 Hz\n"); break;
		case LIS3MDL_DATARATE_155_HZ: printf("155 Hz\n"); break;
		case LIS3MDL_DATARATE_300_HZ: printf("300 Hz\n"); break;
		case LIS3MDL_DATARATE_560_HZ: printf("560 Hz\n"); break;
		case LIS3MDL_DATARATE_1000_HZ: printf("1000 Hz\n"); break;
	}

	lis_sensor->setRange(LIS3MDL_RANGE_4_GAUSS);
	printf("Range set to: \n");
	switch (lis_sensor->getRange()) {
		case LIS3MDL_RANGE_4_GAUSS: printf("+-4 gauss\n"); break;
		case LIS3MDL_RANGE_8_GAUSS: printf("+-8 gauss\n"); break;
		case LIS3MDL_RANGE_12_GAUSS: printf("+-12 gauss\n"); break;
		case LIS3MDL_RANGE_16_GAUSS: printf("+-16 gauss\n"); break;
	}

	lis_sensor->setIntThreshold(500);
	lis_sensor->configInterrupt(false, false, true, // enable z axis
						  true, // polarity
						  false, // don't latch
						  true); // enabled!
	return (res_lis_init & res_bmi_init_acc & res_bmi_init_gyr);

}

bool IMU::initialize_sensors(BMI088_Sens::Config bmi_config, LIS3MDL_Sens::Config lis_config){
	this->bmi_conf = bmi_config;
	this->lis_conf = lis_config;
	return this->initialize_sensors();
}

void IMU::init_ekfs(){

    this->quaternionData.vSetToZero();
    this->quaternionData[0][0] = 1.0;
    this->EKF_IMU.vReset(this->quaternionData, this->EKF_PINIT, this->EKF_QINIT, this->EKF_RINIT);

}

BMI088_Sens::xyz IMU::get_last_linear_accel(){
	return this->last_acc;
}

BMI088_Sens::xyz IMU::get_last_angular_accel(){
	return this->last_gyr;
}

//LIS3MDL_Sens::xyz IMU::get_last_mag() {
//	return this->last_mag;
//}

Adafruit_LIS3MDL::xyz IMU::get_last_mag() {
	return this->last_mag;
}

Adafruit_LIS3MDL::xyz IMU::get_last_mag_cal() {
	return this->last_mag_cal;
}

Quaternion IMU::get_last_attitude(){
    this->upd_attitude();
    return this->last_attitude;
}


void IMU::upd_attitude(){
    this->last_acc = this->bmi_sensor->get_last_accel();
    this->last_gyr = this->bmi_sensor->get_last_angle_accel();
//    this->last_mag = this->lis_sensor->get_last_mag_transformed();
    this->last_mag_cal = this->lis_sensor->get_last_mag_cal();


//	float Ax = this->last_acc.x;
//	float Ay = this->last_acc.y;
//	float Az = this->last_acc.z;
//	float Bx = this->last_mag_cal.y;
//	float By = this->last_mag_cal.x;
//	float Bz = (-1.0)*this->last_mag_cal.z;
//	float p = this->last_gyr.x;
//	float q = this->last_gyr.y;
//	float r = this->last_gyr.z;
//	float Ax = this->last_acc.z;
//	float Ay = this->last_acc.y;
//	float Az = this->last_acc.x;
//	float Bx = this->last_mag_cal.z;
//	float By = this->last_mag_cal.y;
//	float Bz = this->last_mag_cal.x;
//	float p = this->last_gyr.z;
//	float q = this->last_gyr.y;
//	float r = this->last_gyr.x;

//    /* Input 1:3 = gyroscope */
//    U[0][0] = p;  U[1][0] = q;  U[2][0] = r;
//    /* Output 1:3 = accelerometer */
//    Y[0][0] = Ax; Y[1][0] = Ay; Y[2][0] = Az;
//    /* Output 4:6 = magnetometer */
//    Y[3][0] = Bx; Y[4][0] = By; Y[5][0] = Bz;
//
//    /* Normalizing the output vector */
//    float_prec _normG = sqrt((Y[0][0] * Y[0][0]) + (Y[1][0] * Y[1][0]) + (Y[2][0] * Y[2][0]));
//    Y[0][0] = Y[0][0] / _normG;
//    Y[1][0] = Y[1][0] / _normG;
//    Y[2][0] = Y[2][0] / _normG;
//    float_prec _normM = sqrt((Y[3][0] * Y[3][0]) + (Y[4][0] * Y[4][0]) + (Y[5][0] * Y[5][0]));
//    Y[3][0] = Y[3][0] / _normM;
//    Y[4][0] = Y[4][0] / _normM;
//    Y[5][0] = Y[5][0] / _normM;
//
//    TickType_t current_timestamp = xTaskGetTickCount();
//    this->dt = (float)(current_timestamp - this->last_timestamp)/(float(configTICK_RATE_HZ));
//    this->EKF_IMU.bUpdate(this->Y, this->U, this->dt);
//    this->quaternionData = EKF_IMU.GetX();
//    this->last_timestamp = current_timestamp;
//
//    this->last_attitude.w = this->quaternionData[0][0];
//    this->last_attitude.x = this->quaternionData[1][0];
//    this->last_attitude.y = this->quaternionData[2][0];
//    this->last_attitude.z = this->quaternionData[3][0];
//
}

bool IMU::Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U, float dt){

    /* Insert the nonlinear update transformation here
     *          x(k+1) = f[x(k), u(k)]
     *
     * The quaternion update function:
     *  q0_dot = 1/2. * (  0   - p*q1 - q*q2 - r*q3)
     *  q1_dot = 1/2. * ( p*q0 +   0  + r*q2 - q*q3)
     *  q2_dot = 1/2. * ( q*q0 - r*q1 +  0   + p*q3)
     *  q3_dot = 1/2. * ( r*q0 + q*q1 - p*q2 +  0  )
     *
     * Euler method for integration:
     *  q0 = q0 + q0_dot * dT;
     *  q1 = q1 + q1_dot * dT;
     *  q2 = q2 + q2_dot * dT;
     *  q3 = q3 + q3_dot * dT;
     */
    float_prec q0, q1, q2, q3;
    float_prec p, q, r;

    q0 = X[0][0];
    q1 = X[1][0];
    q2 = X[2][0];
    q3 = X[3][0];

    p = U[0][0];
    q = U[1][0];
    r = U[2][0];

    X_Next[0][0] = (0.5 * (+0.00 -p*q1 -q*q2 -r*q3))*dt + q0;
    X_Next[1][0] = (0.5 * (+p*q0 +0.00 +r*q2 -q*q3))*dt + q1;
    X_Next[2][0] = (0.5 * (+q*q0 -r*q1 +0.00 +p*q3))*dt + q2;
    X_Next[3][0] = (0.5 * (+r*q0 +q*q1 -p*q2 +0.00))*dt + q3;


    /* ======= Additional ad-hoc quaternion normalization to make sure the quaternion is a unit vector (i.e. ||q|| = 1) ======= */
    if (!X_Next.bNormVector()) {
        /* System error, return false so we can reset the UKF */
        return false;
    }

    return true;
}


bool IMU::Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U)
{
    /* Insert the nonlinear measurement transformation here
     *          y(k)   = h[x(k), u(k)]
     *
     * The measurement output is the gravitational and magnetic projection to the body:
     *     DCM     = [(+(q0**2)+(q1**2)-(q2**2)-(q3**2)),                    2*(q1*q2+q0*q3),                    2*(q1*q3-q0*q2)]
     *               [                   2*(q1*q2-q0*q3), (+(q0**2)-(q1**2)+(q2**2)-(q3**2)),                    2*(q2*q3+q0*q1)]
     *               [                   2*(q1*q3+q0*q2),                    2*(q2*q3-q0*q1), (+(q0**2)-(q1**2)-(q2**2)+(q3**2))]
     *
     *  G_proj_sens = DCM * [0 0 1]             --> Gravitational projection to the accelerometer sensor
     *  M_proj_sens = DCM * [Mx My Mz]          --> (Earth) magnetic projection to the magnetometer sensor
     */
    float_prec q0, q1, q2, q3;
    float_prec q0_2, q1_2, q2_2, q3_2;

    q0 = X[0][0];
    q1 = X[1][0];
    q2 = X[2][0];
    q3 = X[3][0];

    q0_2 = q0 * q0;
    q1_2 = q1 * q1;
    q2_2 = q2 * q2;
    q3_2 = q3 * q3;

    Y[0][0] = (2*q1*q3 -2*q0*q2) * IMU_ACC_Z0;

    Y[1][0] = (2*q2*q3 +2*q0*q1) * IMU_ACC_Z0;

    Y[2][0] = (+(q0_2) -(q1_2) -(q2_2) +(q3_2)) * IMU_ACC_Z0;

//    Y[3][0] = (+(q0_2)+(q1_2)-(q2_2)-(q3_2)) * IMU_MAG_B0[0][0]
//             +(2*(q1*q2+q0*q3)) * IMU_MAG_B0[1][0]
//             +(2*(q1*q3-q0*q2)) * IMU_MAG_B0[2][0];
//
//    Y[4][0] = (2*(q1*q2-q0*q3)) * IMU_MAG_B0[0][0]
//             +(+(q0_2)-(q1_2)+(q2_2)-(q3_2)) * IMU_MAG_B0[1][0]
//             +(2*(q2*q3+q0*q1)) * IMU_MAG_B0[2][0];
//
//    Y[5][0] = (2*(q1*q3+q0*q2)) * IMU_MAG_B0[0][0]
//             +(2*(q2*q3-q0*q1)) * IMU_MAG_B0[1][0]
//             +(+(q0_2)-(q1_2)-(q2_2)+(q3_2)) * IMU_MAG_B0[2][0];

    Y[3][0] = (+(q0_2)+(q1_2)-(q2_2)-(q3_2)) * 1;

    Y[4][0] = (2*(q1*q2-q0*q3)) * 1;

    Y[5][0] = (2*(q1*q3+q0*q2)) * 1;

    return true;
}

bool IMU::Main_bCalcJacobianF(Matrix& F, const Matrix& X, const Matrix& U, float dt)
{
    /* In Main_bUpdateNonlinearX():
     *  q0 = q0 + q0_dot * dT;
     *  q1 = q1 + q1_dot * dT;
     *  q2 = q2 + q2_dot * dT;
     *  q3 = q3 + q3_dot * dT;
     */
    float_prec p, q, r;

    p = U[0][0];
    q = U[1][0];
    r = U[2][0];

    F[0][0] =  1.000;
    F[1][0] =  0.5*p * dt;
    F[2][0] =  0.5*q * dt;
    F[3][0] =  0.5*r * dt;

    F[0][1] = -0.5*p * dt;
    F[1][1] =  1.000;
    F[2][1] = -0.5*r * dt;
    F[3][1] =  0.5*q * dt;

    F[0][2] = -0.5*q * dt;
    F[1][2] =  0.5*r * dt;
    F[2][2] =  1.000;
    F[3][2] = -0.5*p * dt;

    F[0][3] = -0.5*r * dt;
    F[1][3] = -0.5*q * dt;
    F[2][3] =  0.5*p * dt;
    F[3][3] =  1.000;

    return true;
}

bool IMU::Main_bCalcJacobianH(Matrix& H, const Matrix& X, const Matrix& U, float dt)
{
    /* In Main_bUpdateNonlinearY():
     *
     * The measurement output is the gravitational and magnetic projection to the body:
     *     DCM     = [(+(q0**2)+(q1**2)-(q2**2)-(q3**2)),                    2*(q1*q2+q0*q3),                    2*(q1*q3-q0*q2)]
     *               [                   2*(q1*q2-q0*q3), (+(q0**2)-(q1**2)+(q2**2)-(q3**2)),                    2*(q2*q3+q0*q1)]
     *               [                   2*(q1*q3+q0*q2),                    2*(q2*q3-q0*q1), (+(q0**2)-(q1**2)-(q2**2)+(q3**2))]
     *
     *  G_proj_sens = DCM * [0 0 -g]            --> Gravitational projection to the accelerometer sensor
     *  M_proj_sens = DCM * [Mx My Mz]          --> (Earth) magnetic projection to the magnetometer sensor
     */
    float_prec q0, q1, q2, q3;

    q0 = X[0][0];
    q1 = X[1][0];
    q2 = X[2][0];
    q3 = X[3][0];

    H[0][0] = -2*q2 * IMU_ACC_Z0;
    H[1][0] = +2*q1 * IMU_ACC_Z0;
    H[2][0] = +2*q0 * IMU_ACC_Z0;
//    H[3][0] =  2*q0*IMU_MAG_B0[0][0] + 2*q3*IMU_MAG_B0[1][0] - 2*q2*IMU_MAG_B0[2][0];
//    H[4][0] = -2*q3*IMU_MAG_B0[0][0] + 2*q0*IMU_MAG_B0[1][0] + 2*q1*IMU_MAG_B0[2][0];
//    H[5][0] =  2*q2*IMU_MAG_B0[0][0] - 2*q1*IMU_MAG_B0[1][0] + 2*q0*IMU_MAG_B0[2][0];
    H[3][0] =  2*q0;
    H[4][0] = -2*q3;
    H[5][0] =  2*q2;

    H[0][1] = +2*q3 * IMU_ACC_Z0;
    H[1][1] = +2*q0 * IMU_ACC_Z0;
    H[2][1] = -2*q1 * IMU_ACC_Z0;
//    H[3][1] =  2*q1*IMU_MAG_B0[0][0]+2*q2*IMU_MAG_B0[1][0] + 2*q3*IMU_MAG_B0[2][0];
//    H[4][1] =  2*q2*IMU_MAG_B0[0][0]-2*q1*IMU_MAG_B0[1][0] + 2*q0*IMU_MAG_B0[2][0];
//    H[5][1] =  2*q3*IMU_MAG_B0[0][0]-2*q0*IMU_MAG_B0[1][0] - 2*q1*IMU_MAG_B0[2][0];
    H[3][1] =  2*q1;
    H[4][1] =  2*q2;
    H[5][1] =  2*q3;

    H[0][2] = -2*q0 * IMU_ACC_Z0;
    H[1][2] = +2*q3 * IMU_ACC_Z0;
    H[2][2] = -2*q2 * IMU_ACC_Z0;
//    H[3][2] = -2*q2*IMU_MAG_B0[0][0]+2*q1*IMU_MAG_B0[1][0] - 2*q0*IMU_MAG_B0[2][0];
//    H[4][2] =  2*q1*IMU_MAG_B0[0][0]+2*q2*IMU_MAG_B0[1][0] + 2*q3*IMU_MAG_B0[2][0];
//    H[5][2] =  2*q0*IMU_MAG_B0[0][0]+2*q3*IMU_MAG_B0[1][0] - 2*q2*IMU_MAG_B0[2][0];
    H[3][2] = -2*q2;
    H[4][2] =  2*q1;
    H[5][2] =  2*q0;

    H[0][3] = +2*q1 * IMU_ACC_Z0;
    H[1][3] = +2*q2 * IMU_ACC_Z0;
    H[2][3] = +2*q3 * IMU_ACC_Z0;
//    H[3][3] = -2*q3*IMU_MAG_B0[0][0]+2*q0*IMU_MAG_B0[1][0] + 2*q1*IMU_MAG_B0[2][0];
//    H[4][3] = -2*q0*IMU_MAG_B0[0][0]-2*q3*IMU_MAG_B0[1][0] + 2*q2*IMU_MAG_B0[2][0];
//    H[5][3] =  2*q1*IMU_MAG_B0[0][0]+2*q2*IMU_MAG_B0[1][0] + 2*q3*IMU_MAG_B0[2][0];
    H[3][3] = -2*q3;
    H[4][3] = -2*q0;
    H[5][3] =  2*q1;

    return true;
}


















