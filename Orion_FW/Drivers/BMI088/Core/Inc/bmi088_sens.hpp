/*
 * bmi088.h
 *
 *  Created on: Mar 8, 2023
 *      Author: leo
 */


#ifndef BMI088_CORE_INC_BMI088_SENS_HPP_
#define BMI088_CORE_INC_BMI088_SENS_HPP_

#include <bmi088_sens_defs.hpp>
#include "bmi08_defs.h"
#include "bmi08x.h"
#include "stm32h7xx_hal.h"
//#include "DataStructures.h"

struct BMI088_Sens;

class bmitestclass{
	void test();
};

class BMI088_Sens{
  public:
    struct Config;
    enum class Range_acc;
    enum class Range_gyro;
    enum class Odr;
    enum class Odr_gyro;
    struct xyz;

    BMI088_Sens(I2C_HandleTypeDef* i2c_handle): 
      acc_dev_addr(BMI08_ACCEL_I2C_ADDR_PRIMARY),
      gyro_dev_addr(BMI08_GYRO_I2C_ADDR_PRIMARY),
      i2c(i2c_handle),
	  bias_offset_acc({0.0f, 0.0f, 0.0f}),
	  bias_offset_gyro({0.0f, 0.0f, 0.0f})
  {};

    bool initialize_accel();
    bool initialize_gyro();
    void set_config(const BMI088_Sens::Config& config);
    void get_accel_wdata(float* acc);
    void get_angle_accel_wdata(float* acc);

    BMI088_Sens::xyz get_last_accel();
    BMI088_Sens::xyz get_last_angle_accel();

    BMI088_Sens::xyz get_last_accel_w_bias();
    BMI088_Sens::xyz get_last_angle_accel_w_bias();

    void calibrate_acc(int sec);
    void calibrate_gyro(int sec);

    struct xyz{
    	float x;
    	float y;
    	float z;
    };
    struct Config{
      BMI088_Sens::Odr odr_conf;
      BMI088_Sens::Range_acc range_acc_conf;
      BMI088_Sens::Range_gyro range_gyro_conf;
      BMI088_Sens::Odr_gyro odr_gyro_conf;
      bool self_test;
      bool mode = true;
      bool power = true; 
    };

    enum class Range_acc{
     Mg3,
     Mg6,
     Mg12,
     Mg24
    };

    enum class Range_gyro{
     Dps_2000,
     Dps_1000,
     Dps_500,
     Dps_250,
     Dps_125
    };

    enum class Odr_gyro{
      ODR_2000HZ_BW_532,
      ODR_2000HZ_BW_230,
      ODR_1000HZ_BW_116,
      ODR_400HZ_BW_47,
      ODR_200HZ_BW_64,
      ODR_200HZ_BW_23,
      ODR_100HZ_BW_32,
      ODR_100HZ_BW_12
    };

    enum class Odr{
      ODR_1600HZ_BW_280HZ,
      ODR_1600HZ_BW_234HZ,
      ODR_1600HZ_BW_145HZ,
      ODR_800HZ_BW_230HZ,
      ODR_800HZ_BW_140HZ,
      ODR_800HZ_BW_80HZ,
      ODR_400HZ_BW_145HZ,
      ODR_400HZ_BW_75HZ,
      ODR_400HZ_BW_40HZ,
      ODR_200HZ_BW_80HZ,
      ODR_200HZ_BW_38HZ,
      ODR_200HZ_BW_20HZ,
      ODR_100HZ_BW_40HZ,
      ODR_100HZ_BW_19HZ,
      ODR_100HZ_BW_10HZ,
      ODR_50HZ_BW_20HZ,
      ODR_50HZ_BW_9HZ,
      ODR_50HZ_BW_5HZ,
      ODR_25HZ_BW_10HZ,
      ODR_25HZ_BW_5HZ,
      ODR_25HZ_BW_3HZ,
      ODR_12_5HZ_BW_5HZ,
      ODR_12_5HZ_BW_2HZ,
      ODR_12_5HZ_BW_1HZ
    };

  private:
    bool chip_id_ok_acc();
    bool soft_reset_acc();
    bool self_test_acc();
    bool set_mode_acc(const bool& mode);
    bool set_range_acc(const BMI088_Sens::Range_acc& range);
    bool set_odr_acc(const BMI088_Sens::Odr& odr);
    bool set_power_acc(const bool& pow);
    bool read_reg(uint8_t reg_addr, uint8_t* data, uint16_t len, uint8_t d_addr);
    bool write_reg(uint8_t reg_addr, uint8_t* data, uint16_t len, uint8_t d_addr);
    void get_accel();

    bool chip_id_ok_gyro();
    bool soft_reset_gyro();
    bool set_range_gyro(const BMI088_Sens::Range_gyro& range_gyro);
    bool set_drdy_int_gyro(bool enable);
    bool set_odr_gyro(const BMI088_Sens::Odr_gyro& range_gyro);
    void get_angle_accel();

	  I2C_HandleTypeDef* i2c;
    uint8_t acc_dev_addr;
    uint8_t gyro_dev_addr;
    float buffer[6];


    BMI088_Sens::Config config;
    float accel_mss[3];
    float angle_accel_radss[3];
    int range_integer;
    int gyro_range_integer;
    const float D2R = M_PI/180.0f;
    float tr_x[3] = {};
    float tr_y[3] = {};
    float tr_z[3] = {};

    BMI088_Sens::xyz bias_offset_acc;
    BMI088_Sens::xyz bias_offset_gyro;

};


///*!
// * @brief Configures all the necessary parameters. First function to call
// * to initialize the accelerometer.
// */
//void initialize_accel(struct BMI088_Sens* sens);
//
///*!
// * @brief Configures all the necessary parameters. First function to
// * call to initialize the gyroscope.
// * @return Returns 0 if all OK. Returns -1 if get_gyro_chip_id() was not successful.
// * Returns -2 if set_range_gyro() was not successful. Returns -3 if set_drdy_int_gyro()
// * was not successful.
// */
//int initialize_gyro(struct BMI088_Sens* sens);
//
///*!
// *@brief Obtain the chip id of the accelerometer (performed in order to check whether it is alive)
// */
//uint8_t get_acc_chip_id(struct BMI088_Sens* sens);
//
///*!
// * @brief Obtain the chip id of the gyroscope (performed in order to check whether the comm. is ok)
// */
//uint8_t get_gyro_chip_id(struct BMI088_Sens* sens);
//
///*!
// * @brief Gets the error register
// */
//uint8_t get_error_acc(struct BMI088_Sens* sens);
//
///*!
// * @brief Gets the fatal error register
// */
//uint8_t get_fatal_error_acc(struct BMI088_Sens* sens);
//
///*!
// * @brief Helper functions for reading the register
// */
//int8_t read_reg(I2C_HandleTypeDef *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
///*!
// * @brief Helper functions for writing to register
// */
//int8_t write_reg(I2C_HandleTypeDef *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
///*!
// * @brief Reads the accel in ms & stores it in the BMI088_Sens structure.
// */
//void get_accel(struct BMI088_Sens* sens, bool mg);
///*!
// * @brief Reads the accel in ms & stores it in the *data
// */
//void get_accel_wdata(struct BMI088_Sens* sens, float* data);
///*!
// * @brief Reads the temperature & stores it in the BMI088_Sens struct
// */
//void get_temperature(struct BMI088_Sens* sens);
//
///*!
// * @brief Performs the self-test for the accelerometer.
// */
//bool self_test_acc(struct BMI088_Sens* sens);
///*!
// * @brief Configures the power (on/off).
// */
//bool set_power_acc(struct BMI088_Sens* sens, bool power);
///*!
// * @brief Sets the power mode.
// */
//bool set_mode_acc(struct BMI088_Sens* sens, bool mode);
///*!
// * @brief Helper function to set the transformation functions.
// */
//void set_transformations_acc(struct BMI088_Sens* sens);
//
///*!
// * @brief Sets the output data rate.
// */
//void set_odr_acc(struct BMI088_Sens* sens, enum Odr odr);
//
///*!
// * @brief Sets the acceleration range.
// */
//bool set_range_acc(struct BMI088_Sens* sens, uint8_t range);
//
///*!
// * @brief Sets the gyroscope range.
// */
//bool set_range_gyro(struct BMI088_Sens* sens, uint8_t range);
//
///*!
// * @brief Performs the self-test for the accelerometer.
// */
//bool soft_reset_acc(struct BMI088_Sens* sens);
//
///*!
// * @brief Performs the self-test for the gyroscope.
// */
//bool soft_reset_gyro(struct BMI088_Sens* sens);
//
///*!
// * @brief Enable data ready int for gyroscope.
// */
//bool set_drdy_int_gyro(struct BMI088_Sens* sens, bool enable);
//
//bool set_odr_gyro(struct BMI088_Sens* sens, uint8_t odr);


#endif BMI088_CORE_INC_BMI088_SENS_HPP_
