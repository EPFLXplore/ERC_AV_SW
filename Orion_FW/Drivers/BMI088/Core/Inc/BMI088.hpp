/*
 * bmi088.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Leo Kruglikov, Vincent Nguyen
 */


#ifndef BMI088_CORE_INC_BMI088_HPP_
#define BMI088_CORE_INC_BMI088_HPP_

#include <BMI088_defs.hpp>
#include "stm32h7xx_hal.h"
#include "DataStructures.h"
#include <vector>

enum acc_scale_type_t { // measurement rage
    RANGE_3G = 0x00,
    RANGE_6G = 0x01,
    RANGE_12G = 0x02,
    RANGE_24G = 0x03,
};

enum acc_odr_type_t { // output data rate and bandwidth
	ODR_1600HZ_BW_280HZ = (0x0A << 4) | 0x0C,
	ODR_1600HZ_BW_234HZ = (0x09 << 4) | 0x0C,
	ODR_1600HZ_BW_145HZ = (0x08 << 4) | 0x0C,
	ODR_800HZ_BW_230HZ  = (0x0A << 4) | 0x0B,
	ODR_800HZ_BW_140HZ  = (0x09 << 4) | 0x0B,
	ODR_800HZ_BW_80HZ   = (0x08 << 4) | 0x0B,
	ODR_400HZ_BW_75HZ   = (0x09 << 4) | 0x0A,
	ODR_400HZ_BW_40HZ   = (0x08 << 4) | 0x0A,
	ODR_200HZ_BW_80HZ   = (0x0A << 4) | 0x09,
	ODR_200HZ_BW_38HZ   = (0x09 << 4) | 0x09,
	ODR_200HZ_BW_20HZ   = (0x08 << 4) | 0x09,
	ODR_100HZ_BW_40HZ   = (0x0A << 4) | 0x08,
	ODR_100HZ_BW_19HZ   = (0x09 << 4) | 0x08,
	ODR_100HZ_BW_10HZ   = (0x08 << 4) | 0x08,
	ODR_50HZ_BW_20HZ    = (0x0A << 4) | 0x07,
	ODR_50HZ_BW_9HZ     = (0x09 << 4) | 0x07,
	ODR_50HZ_BW_5HZ     = (0x08 << 4) | 0x07,
	ODR_25HZ_BW_10HZ    = (0x0A << 4) | 0x06,
	ODR_25HZ_BW_5HZ     = (0x09 << 4) | 0x06,
	ODR_25HZ_BW_3HZ     = (0x08 << 4) | 0x06,
	ODR_12_5HZ_BW_5HZ   = (0x0A << 4) | 0x05,
	ODR_12_5HZ_BW_2HZ   = (0x09 << 4) | 0x05,
	ODR_12_5HZ_BW_1HZ   = (0x08 << 4) | 0x05
};

enum acc_mode_type_t { // power mode
    ACC_ACTIVE = 0x00, //
    ACC_SUSPEND = 0x03, //
};

enum acc_power_type_t { // power mode
    ACC_ENABLE = 0x04, //
    ACC_DISABLE = 0x00, //
};

enum gyro_scale_type_t { // measurement rage
    RANGE_2000 = 0x00, //
    RANGE_1000 = 0x01, //
    RANGE_500 = 0x02, //
    RANGE_250 = 0x03, //
    RANGE_125 = 0x04, //
};

enum gyro_odr_type_t { // output data rate
    ODR_2000HZ_BW_532 = 0x00, //
    ODR_2000HZ_BW_230 = 0x01, //
    ODR_1000HZ_BW_116 = 0x02, //
    ODR_400HZ_BW_47 = 0x03, //
    ODR_200HZ_BW_23 = 0x04, //
    ODR_100HZ_BW_12 = 0x05, //
    ODR_200HZ_BW_64 = 0x06, //
    ODR_100HZ_BW_32 = 0x07, //
};

enum gyro_power_type_t { // power mode
    GYRO_NORMAL = 0x00, //
    GYRO_SUSPEND = 0x80, //
    GYRO_DEEP_SUSPEND = 0x20, //
};

enum gyro_drdy_int_type_t {
	GYRO_ENABLE_DRDY_INT = 0x80,
	GYRO_DISABLE_DRDY_INT = 0x00,
};

class BMI088{
  public:
    struct Config;
    BMI088(I2C_HandleTypeDef* i2c_handle, Config conf_):
        i2c(i2c_handle),
		acc_dev_addr(BMI08_ACCEL_I2C_ADDR_PRIMARY),
		gyro_dev_addr(BMI08_GYRO_I2C_ADDR_PRIMARY),
		config(conf_),
		ACC_BIAS{-0.041430, 0.117302, -0.016241},
		GYRO_BIAS{0.00663f, -0.0005f, -0.00166f},
		ACC_TF({{0.999371, -0.000365, -0.000939},
            	{-0.000365, 1.001050, 0.000545},
				{-0.000939, 0.000545, 0.995865}})
  {};

    HAL_StatusTypeDef initialize_accel();
    HAL_StatusTypeDef initialize_gyro();

    HAL_StatusTypeDef get_accel(Vector& acc_);
    HAL_StatusTypeDef get_accel_cal(Vector& acc_);
    HAL_StatusTypeDef get_accel_raw(Vector& acc_);

    HAL_StatusTypeDef get_gyro(Vector& gyro_);
    HAL_StatusTypeDef get_gyro_cal(Vector& gyro_);
    HAL_StatusTypeDef get_gyro_raw(Vector& gyro_);

    HAL_StatusTypeDef compute_gyro_bias(Vector& bias_, uint32_t times = 10000);

//    void calibrate_acc(int sec);
//    void calibrate_gyro(int sec);

    Vector acc;
    Vector acc_mss;
    Vector acc_cal_mss;
    Vector gyro;
    Vector gyro_rads;
    Vector gyro_cal_rads;

    struct Config{
    	acc_odr_type_t odr_acc_conf;
    	acc_scale_type_t range_acc_conf;
    	gyro_odr_type_t odr_gyro_conf;
    	gyro_scale_type_t range_gyro_conf;
    };

	private:
    	HAL_StatusTypeDef read_reg(uint8_t reg_addr, uint8_t* data, uint16_t len, uint8_t dev_addr);
    	HAL_StatusTypeDef read_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift, uint8_t dev_addr);
    	HAL_StatusTypeDef write_reg(uint8_t reg_addr, uint8_t data, uint8_t dev_addr);

        HAL_StatusTypeDef read_accel();
    	HAL_StatusTypeDef read_gyro();

    	HAL_StatusTypeDef chip_id_ok_acc();
    	HAL_StatusTypeDef soft_reset_acc();
    	HAL_StatusTypeDef self_test_acc();
    	HAL_StatusTypeDef check_fatal_err_acc();
    	HAL_StatusTypeDef check_config_err_acc();
    	HAL_StatusTypeDef set_mode_acc(acc_mode_type_t type);
    	HAL_StatusTypeDef set_range_acc(acc_scale_type_t range);
    	HAL_StatusTypeDef set_odr_acc(acc_odr_type_t odr);
    	HAL_StatusTypeDef set_power_acc(acc_power_type_t power);

    	HAL_StatusTypeDef chip_id_ok_gyro();
    	HAL_StatusTypeDef soft_reset_gyro();
    	HAL_StatusTypeDef set_range_gyro(gyro_scale_type_t range);
    	HAL_StatusTypeDef set_drdy_int_gyro(gyro_drdy_int_type_t type);
    	HAL_StatusTypeDef set_odr_gyro(gyro_odr_type_t odr);

    	I2C_HandleTypeDef* i2c;
		uint8_t acc_dev_addr;
		uint8_t gyro_dev_addr;


		Config config;

		uint16_t acc_range;
		uint16_t gyro_range;
		const float D2R = M_PI/180.0f;
		float ACC_BIAS[3];
		float GYRO_BIAS[3];
		std::vector<std::vector<float>> ACC_TF;

};


#endif BMI088_CORE_INC_BMI088_HPP_
