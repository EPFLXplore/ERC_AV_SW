/*
 * LIS3MDL.hpp
 *
 *  Created on: Jul 26, 2023
 *      Author: Vincent
 */

#ifndef LIS3MDL_CORE_INC_LIS3MDL_HPP_
#define LIS3MDL_CORE_INC_LIS3MDL_HPP_
#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <vector>
#include "matrix.h"
#include "konfig.h"

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define LIS3MDL_I2CADDR_DEFAULT (0x1E) ///< Default breakout addres
/*=========================================================================*/

#define LIS3MDL_REG_WHO_AM_I 0x0F  ///< Register that contains the part ID
#define LIS3MDL_REG_CTRL_REG1 0x20 ///< Register address for control 1
#define LIS3MDL_REG_CTRL_REG2 0x21 ///< Register address for control 2
#define LIS3MDL_REG_CTRL_REG3 0x22 ///< Register address for control 3
#define LIS3MDL_REG_CTRL_REG4 0x23 ///< Register address for control 3
#define LIS3MDL_REG_STATUS 0x27    ///< Register address for status
#define LIS3MDL_REG_OUT_X_L 0x28   ///< Register address for X axis lower byte
#define LIS3MDL_REG_INT_CFG 0x30   ///< Interrupt configuration register
#define LIS3MDL_REG_INT_THS_L 0x32 ///< Low byte of the irq threshold

/** The magnetometer ranges */
typedef enum {
  LIS3MDL_RANGE_4_GAUSS = 0b00,  ///< +/- 4g (default value)
  LIS3MDL_RANGE_8_GAUSS = 0b01,  ///< +/- 8g
  LIS3MDL_RANGE_12_GAUSS = 0b10, ///< +/- 12g
  LIS3MDL_RANGE_16_GAUSS = 0b11, ///< +/- 16g
} lis3mdl_range_t;

/** The magnetometer data rate, includes FAST_ODR bit */
typedef enum {
  LIS3MDL_DATARATE_0_625_HZ = 0b0000, ///<  0.625 Hz
  LIS3MDL_DATARATE_1_25_HZ = 0b0010,  ///<  1.25 Hz
  LIS3MDL_DATARATE_2_5_HZ = 0b0100,   ///<  2.5 Hz
  LIS3MDL_DATARATE_5_HZ = 0b0110,     ///<  5 Hz
  LIS3MDL_DATARATE_10_HZ = 0b1000,    ///<  10 Hz
  LIS3MDL_DATARATE_20_HZ = 0b1010,    ///<  20 Hz
  LIS3MDL_DATARATE_40_HZ = 0b1100,    ///<  40 Hz
  LIS3MDL_DATARATE_80_HZ = 0b1110,    ///<  80 Hz
  LIS3MDL_DATARATE_155_HZ = 0b0001,   ///<  155 Hz (FAST_ODR + UHP)
  LIS3MDL_DATARATE_300_HZ = 0b0011,   ///<  300 Hz (FAST_ODR + HP)
  LIS3MDL_DATARATE_560_HZ = 0b0101,   ///<  560 Hz (FAST_ODR + MP)
  LIS3MDL_DATARATE_1000_HZ = 0b0111,  ///<  1000 Hz (FAST_ODR + LP)
} lis3mdl_dataRate_t;

/** The magnetometer performance mode */
typedef enum {
  LIS3MDL_LOWPOWERMODE = 0b00,  ///< Low power mode
  LIS3MDL_MEDIUMMODE = 0b01,    ///< Medium performance mode
  LIS3MDL_HIGHMODE = 0b10,      ///< High performance mode
  LIS3MDL_ULTRAHIGHMODE = 0b11, ///< Ultra-high performance mode
} lis3mdl_performancemode_t;

/** The magnetometer operation mode */
typedef enum {
  LIS3MDL_CONTINUOUSMODE = 0b00, ///< Continuous conversion
  LIS3MDL_SINGLEMODE = 0b01,     ///< Single-shot conversion
  LIS3MDL_POWERDOWNMODE = 0b11,  ///< Powered-down mode
} lis3mdl_operationmode_t;

/** Class for hardware interfacing with an LIS3MDL magnetometer */
class Adafruit_LIS3MDL {
public:
  Adafruit_LIS3MDL(I2C_HandleTypeDef* i2c):
	  i2c(i2c),
	  dev_addr(LIS3MDL_I2CADDR_DEFAULT),
	  HARD_IRON{0.834208, 0.022749, -0.015229},
	  SOFT_IRON({{0.834208, 0.022749, -0.015229},
          	  	 {0.022749, 0.861209, 0.003485},
				 {-0.015229, 0.003485, 0.838099}}) {};

  HAL_StatusTypeDef read_reg(uint8_t reg, uint8_t* data, uint16_t len);
  HAL_StatusTypeDef read_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift);
  HAL_StatusTypeDef write_reg(uint8_t reg, uint8_t* data, uint16_t len);
  HAL_StatusTypeDef write_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift);

  HAL_StatusTypeDef init();

  void reset(void);

  void setPerformanceMode(lis3mdl_performancemode_t mode);
  lis3mdl_performancemode_t getPerformanceMode(void);
  void setOperationMode(lis3mdl_operationmode_t mode);
  lis3mdl_operationmode_t getOperationMode(void);
  void setDataRate(lis3mdl_dataRate_t dataRate);
  lis3mdl_dataRate_t getDataRate(void);
  void setRange(lis3mdl_range_t range);
  lis3mdl_range_t getRange(void);
  void setIntThreshold(uint16_t value);
  uint16_t getIntThreshold(void);
  void configInterrupt(bool enableX, bool enableY, bool enableZ, bool polarity,
                       bool latch, bool enableInt);
  void selfTest(bool flag);

  void read();

  struct xyz{
      	float x;
      	float y;
      	float z;
      };
  Adafruit_LIS3MDL::xyz get_last_mag();
  Adafruit_LIS3MDL::xyz get_last_mag_cal();

  int readMagneticField(float &x, float &y, float &z);
  float magneticFieldSampleRate(void);
  int magneticFieldAvailable(void);

  int16_t x,     ///< The last read X mag in raw units
      y,         ///< The last read Y mag in raw units
      z;         ///< The last read Z mag in raw units
  float x_gauss, ///< The last read X mag in 'gauss'
      y_gauss,   ///< The last read Y mag in 'gauss'
      z_gauss;   ///< The last read Z mag in 'gauss'

  float x_uT,    ///< The last read X mag in 'uTesla'
  	  y_uT,		 ///< The last read X mag in 'uTesla'
	  z_uT;		 ///< The last read X mag in 'uTesla'

  float x_cal_uT,
  	  y_cal_uT,
	  z_cal_uT;
  //! buffer for the magnetometer range
  lis3mdl_range_t rangeBuffered = LIS3MDL_RANGE_4_GAUSS;

//  float_prec HARD_IRON_data[3*1];
//  float_prec SOFT_IRON_data[3*3];
//
//  Matrix HARD_IRON;
//  Matrix SOFT_IRON;
  float HARD_IRON[3];
  std::vector<std::vector<float>> SOFT_IRON;

private:
  HAL_StatusTypeDef _init(void);

  I2C_HandleTypeDef* i2c;
  uint8_t dev_addr;

  int32_t _sensorID;
};

#endif /* LIS3MDL_CORE_INC_LIS3MDL_HPP_ */
