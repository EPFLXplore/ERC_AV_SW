/*
 * lis3mdl_sens.h
 *
 *  Created on: Mar 23, 2023
 *      Author: leo
 */

#ifndef LIS3MDL_CORE_INC_LIS3MDL_SENS_HPP_
#define LIS3MDL_CORE_INC_LIS3MDL_SENS_HPP_
#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <vector>

#define LIS3_WHO_AM_I 0x0F
#define LIS3_CTRL_REG1 0x20
#define LIS3_CTRL_REG2 0x21
#define LIS3_CTRL_REG3 0x22
#define LIS3_CTRL_REG4 0x23
#define LIS3_CTRL_REG5 0x24
#define LIS3_STATUS_REG 0x27
#define LIS3_OUT_X_L 0x28
#define LIS3_OUT_X_H 0x29
#define LIS3_OUT_Y_L 0x2A
#define LIS3_OUT_Y_H 0x2B
#define LIS3_OUT_Z_L 0x2C
#define LIS3_OUT_Z_H 0x2D
#define LIS3_TEMP_OUT_L 0x2E
#define LIS3_TEMP_OUT_H 0x2F
#define LIS3_INT_CFG 0x30
#define LIS3_INT_SRC 0x31
#define LIS3_INT_THS_L 0x32
#define LIS3_INT_THS_H 0x33

#define LIS3_I2C_ADDR 0x1E
#define LIS3_CHIP_ID 0x3D

//class test_class{

//};

class LIS3MDL_Sens{
  public:
    enum class LIS3MDL_Odr{
      Odr_1000Hz,
	  Odr_560Hz,
	  Odr_300Hz,
	  Odr_155Hz
    };
    enum class LIS3MDL_Range{
    	Range_4,
		Range_8,
		Range_12,
		Range_16
    };
    struct Config{
      LIS3MDL_Sens::LIS3MDL_Odr odr_config;
      LIS3MDL_Sens::LIS3MDL_Range range_config;
      bool self_test_enable;
      bool reboot;
      bool soft_reset;
      bool cont_conv_mode;
      bool fast_read;
    };
    struct xyz{
    	float x;
    	float y;
    	float z;
    };
    LIS3MDL_Sens(I2C_HandleTypeDef* i2c):i2c(i2c), dev_addr(0x1E){};
    LIS3MDL_Sens::xyz get_last_mag();
    LIS3MDL_Sens::xyz get_last_mag_transformed();
    void set_config(LIS3MDL_Sens::Config conf);
    bool set_odr(const LIS3MDL_Sens::LIS3MDL_Odr& odr);
    bool set_range(const LIS3MDL_Sens::LIS3MDL_Range& range);
    bool set_self_test(const bool& self_test);
    bool set_reboot(const bool& reboot);
    bool set_soft_reset(const bool& soft_reset);
    bool set_cont_conv_mode(const bool& cont_conv);
    bool set_z_odr(const LIS3MDL_Sens::LIS3MDL_Odr& odr);
    /*!
     * @brief Sets the fast read and blocking mechanism
     */
    bool set_fast_read(const bool& fast_read);
    bool init();



  private:
    I2C_HandleTypeDef* i2c;
    bool read_reg(uint8_t reg_addr, uint8_t* data, uint16_t len);
    bool write_reg(uint8_t reg_addr, uint8_t* data, uint16_t len);
    bool chip_id_ok(); 
    void get_data();
    void matrix_vec_multiply(const std::vector<std::vector<float>>& matrix,
    		const float* vector, float* result);

    LIS3MDL_Sens::Config config;
    uint8_t dev_addr;
    float mag_val[3];
    float mag_scale_real;
    float mag_bias_offset[3];
    std::vector<std::vector<float>> mag_transformation_matrix;

};

//struct LIS3MDL_Sens{
//	I2C_HandleTypeDef* i2c;
//	uint8_t dev_addr;
//	uint8_t mag_val[3];
//};
//
//enum LIS3MDL_ODR_CONF{
//	LIS3MDL_ODR_1000,
//	LIS3MDL_ODR_560,
//	LIS3MDL_ODR_300,
//	LIS3MDL_ODR_155,
//	LIS3MDL_ODR_80,
//	LIS3MDL_ODR_40,
//	LIS3MDL_ODR_20,
//	LIS3MDL_ODR_10
//};
//
//bool lis3mdl_check_alive(struct LIS3MDL_Sens* sens);
//
//bool lis3mdl_config_odr(struct LIS3MDL_Sens* sens, enum LIS3MDL_ODR_CONF odr);
//
//void lis3mdl_get_data(struct LIS3MDL_Sens* sens);
//
//
//#endif /* LIS3MDL_CORE_INC_LIS3MDL_SENS_HPP_ */

#endif
