/*
 * LIS3MDL.cpp
 *
 *  Created on: Jul 26, 2023
 *      Author: Vincent
 */

#include "LIS3MDL.hpp"
#include <cmsis_os.h>

HAL_StatusTypeDef Adafruit_LIS3MDL::read_reg(uint8_t reg, uint8_t* data, uint16_t len) {
	vTaskSuspendAll();
	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Read(this->i2c, this->dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
	__enable_irq();
	xTaskResumeAll();
	return res;
}

HAL_StatusTypeDef Adafruit_LIS3MDL::read_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift) {
    if (data == nullptr || bits > 8 || shift > 7) {
        return HAL_ERROR; // Invalid parameters
    }

    uint8_t reg_value;
    HAL_StatusTypeDef res;

    // Read the register value using HAL_I2C_Mem_Read
    res = read_reg(reg, &reg_value, 1);
    if (res != HAL_OK) {
        return res; // Error occurred during the I2C read
    }

    // Mask the bits we want to keep (set the other bits to 0)
    uint8_t mask = (1 << bits) - 1;

    // Shift and extract the desired bits
    *data = (reg_value >> shift) & mask;

    return HAL_OK;
}

HAL_StatusTypeDef Adafruit_LIS3MDL::write_reg(uint8_t reg, uint8_t* data, uint16_t len) {
	vTaskSuspendAll();
	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Write(this->i2c, this->dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
	__enable_irq();
	xTaskResumeAll();

	return res;
}

HAL_StatusTypeDef Adafruit_LIS3MDL::write_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift) {
    // Make sure the input parameters are valid
    if (data == nullptr || bits > 8 || shift > 7) {
        return HAL_ERROR; // Invalid parameters
    }

    HAL_StatusTypeDef res;
    uint8_t reg_value;

    // Read the current register value using HAL_I2C_Mem_Read
    res = read_reg(reg, &reg_value, 1);
    if (res != HAL_OK) {
        return res; // Error occurred during the I2C read
    }

    // Create a bitmask for the bits we want to modify (set to 1)
    uint8_t mask = ((1 << bits) - 1) << shift;

    // Clear the bits we want to modify in the register value
    reg_value &= ~mask;

    // Set the new data bits at the specified position
    reg_value |= ((*data & ((1 << bits) - 1)) << shift);

    // Write the modified register value using HAL_I2C_Mem_Write
    res = write_reg(reg, &reg_value, 1);

    return res;
}

HAL_StatusTypeDef Adafruit_LIS3MDL::init() {
	// load config


	return _init();
}

HAL_StatusTypeDef Adafruit_LIS3MDL::_init(void) {
  // Check connection
  uint8_t chip_id;
  HAL_StatusTypeDef res;
  res = read_reg(LIS3MDL_REG_WHO_AM_I, &chip_id, 1);

  if (res != HAL_OK)
	  return res;

  // make sure we're talking to the right chip
  if (chip_id != 0x3D) {
    // No LIS3MDL detected ... return HAL_ERROR
    return HAL_ERROR;
  }

  reset();

  // set high quality performance mode
  setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);

  // 155Hz default rate
  setDataRate(LIS3MDL_DATARATE_155_HZ);

  // lowest range
  setRange(LIS3MDL_RANGE_4_GAUSS);

  setOperationMode(LIS3MDL_CONTINUOUSMODE);

  return HAL_OK;
}

/**************************************************************************/
/*!

@brief  Performs a software reset
*/
/**************************************************************************/
void Adafruit_LIS3MDL::reset(void) {
  write_bits(LIS3MDL_REG_CTRL_REG2, (uint8_t*)0x1, 1, 2);
  osDelay(10);
  getRange();
}

/**************************************************************************/
/*!
  @brief  Read the XYZ data from the magnetometer and store in the internal
  x, y and z (and x_g, y_g, z_g) member variables.
*/
/**************************************************************************/

void Adafruit_LIS3MDL::read(void) {
  uint8_t buffer[6];
  read_reg(LIS3MDL_REG_OUT_X_L, buffer, 6);

  x = buffer[0];
  x |= buffer[1] << 8;
  y = buffer[2];
  y |= buffer[3] << 8;
  z = buffer[4];
  z |= buffer[5] << 8;

  float scale = 1; // LSB per gauss
  switch (rangeBuffered) {
  case LIS3MDL_RANGE_16_GAUSS:
    scale = 1711;
    break;
  case LIS3MDL_RANGE_12_GAUSS:
    scale = 2281;
    break;
  case LIS3MDL_RANGE_8_GAUSS:
    scale = 3421;
    break;
  case LIS3MDL_RANGE_4_GAUSS:
    scale = 6842;
    break;
  }

  x_gauss = (float)x / scale;
  y_gauss = (float)y / scale;
  z_gauss = (float)z / scale;
}

/**************************************************************************/
/*!
    @brief Set the performance mode, LIS3MDL_LOWPOWERMODE, LIS3MDL_MEDIUMMODE,
    LIS3MDL_HIGHMODE or LIS3MDL_ULTRAHIGHMODE
    @param mode Enumerated lis3mdl_performancemode_t
*/
/**************************************************************************/
void Adafruit_LIS3MDL::setPerformanceMode(lis3mdl_performancemode_t mode) {
  // write xy
  write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&mode), 2, 5);

  // write z
  write_bits(LIS3MDL_REG_CTRL_REG4, (uint8_t*)(&mode), 2, 2);
}

/**************************************************************************/
/*!
    @brief Get the performance mode
    @returns Enumerated lis3mdl_performancemode_t, LIS3MDL_LOWPOWERMODE,
    LIS3MDL_MEDIUMMODE, LIS3MDL_HIGHMODE or LIS3MDL_ULTRAHIGHMODE
*/
/**************************************************************************/
lis3mdl_performancemode_t Adafruit_LIS3MDL::getPerformanceMode(void) {
  uint8_t performance;
  read_bits(LIS3MDL_REG_CTRL_REG1, &performance, 2, 5);
  return (lis3mdl_performancemode_t)performance;
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the LIS3MDL (controls power consumption)
    from 0.625 Hz to 80Hz
    @param dataRate Enumerated lis3mdl_dataRate_t
*/
/**************************************************************************/
void Adafruit_LIS3MDL::setDataRate(lis3mdl_dataRate_t dataRate) {
  if (dataRate == LIS3MDL_DATARATE_155_HZ) {
    // set OP to UHP
    setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  }
  if (dataRate == LIS3MDL_DATARATE_300_HZ) {
    // set OP to HP
    setPerformanceMode(LIS3MDL_HIGHMODE);
  }
  if (dataRate == LIS3MDL_DATARATE_560_HZ) {
    // set OP to MP
    setPerformanceMode(LIS3MDL_MEDIUMMODE);
  }
  if (dataRate == LIS3MDL_DATARATE_1000_HZ) {
    // set OP to LP
    setPerformanceMode(LIS3MDL_LOWPOWERMODE);
  }
  osDelay(10);
  write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&dataRate), 4, 1);
}

/**************************************************************************/
/*!
    @brief  Gets the data rate for the LIS3MDL (controls power consumption)
    @return Enumerated lis3mdl_dataRate_t from 0.625 Hz to 80Hz
*/
/**************************************************************************/
lis3mdl_dataRate_t Adafruit_LIS3MDL::getDataRate(void) {
  uint8_t datarate;
  read_bits(LIS3MDL_REG_CTRL_REG1, &datarate, 4, 1);
  return (lis3mdl_dataRate_t)datarate;
}

/**************************************************************************/
/*!
    @brief Set the operation mode, LIS3MDL_CONTINUOUSMODE,
    LIS3MDL_SINGLEMODE or LIS3MDL_POWERDOWNMODE
    @param mode Enumerated lis3mdl_operationmode_t
*/
/**************************************************************************/
void Adafruit_LIS3MDL::setOperationMode(lis3mdl_operationmode_t mode) {
  // write x and y
  write_bits(LIS3MDL_REG_CTRL_REG3, (uint8_t*)(&mode), 2, 0);
}


/**************************************************************************/
/*!
    @brief Get the operation mode
    @returns Enumerated lis3mdl_operationmode_t, LIS3MDL_CONTINUOUSMODE,
    LIS3MDL_SINGLEMODE or LIS3MDL_POWERDOWNMODE
*/
/**************************************************************************/
lis3mdl_operationmode_t Adafruit_LIS3MDL::getOperationMode(void) {
  uint8_t opmode;
  read_bits(LIS3MDL_REG_CTRL_REG3, &opmode, 2, 0);
  return (lis3mdl_operationmode_t)opmode;
}


/**************************************************************************/
/*!
    @brief Set the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @param range Enumerated lis3mdl_range_t
*/
/**************************************************************************/
void Adafruit_LIS3MDL::setRange(lis3mdl_range_t range) {
  write_bits(LIS3MDL_REG_CTRL_REG2, (uint8_t*)(&range), 2, 5);
  rangeBuffered = range;
}

/**************************************************************************/
/*!
    @brief Read the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @returns Enumerated lis3mdl_range_t
*/
/**************************************************************************/
lis3mdl_range_t Adafruit_LIS3MDL::getRange(void) {
  uint8_t range;
  read_bits(LIS3MDL_REG_CTRL_REG2, &range, 2, 5);

  rangeBuffered = (lis3mdl_range_t)range;

  return rangeBuffered;
}

/**************************************************************************/
/*!
    @brief Set the interrupt threshold value
    @param value 16-bit unsigned raw value
*/
/**************************************************************************/
void Adafruit_LIS3MDL::setIntThreshold(uint16_t value) {
  value &= 0x7FFF; // high bit must be 0!
  write_reg(LIS3MDL_REG_INT_THS_L, (uint8_t*)value, 2);
}

/**************************************************************************/
/*!
    @brief Get the interrupt threshold value
    @returns 16-bit unsigned raw value
*/
/**************************************************************************/
uint16_t Adafruit_LIS3MDL::getIntThreshold(void) {
//  uint8_t int_threshold[2];
//  read_reg(LIS3MDL_REG_INT_THS_L, int_threshold, 2);
//  return (uint16_t)((int_threshold[1] << 8) | int_threshold[0]);
	uint16_t int_threshold;
	read_reg(LIS3MDL_REG_INT_THS_L, (uint8_t*)&int_threshold, 2);
	return int_threshold;
}

/**************************************************************************/
/*!
    @brief Configure INT_CFG
    @param enableX Enable interrupt generation on X-axis
    @param enableY Enable interrupt generation on Y-axis
    @param enableZ Enable interrupt generation on Z-axis
    @param polarity Sets the polarity of the INT output logic
    @param latch If true (latched) the INT pin remains in the same state
    until INT_SRC is read.
    @param enableInt Interrupt enable on INT pin
*/
/**************************************************************************/
void Adafruit_LIS3MDL::configInterrupt(bool enableX, bool enableY, bool enableZ,
                                       bool polarity, bool latch,
                                       bool enableInt) {
  uint8_t value = 0x08; // set default bits, see table 36
  value |= enableX << 7;
  value |= enableY << 6;
  value |= enableZ << 5;
  value |= polarity << 2;
  value |= latch << 1;
  value |= enableInt;

  write_reg(LIS3MDL_REG_INT_CFG, &value, 1);
}

/**************************************************************************/
/*!
    @brief Enable or disable self-test
    @param flag If true, enable self-test

*/
/**************************************************************************/
void Adafruit_LIS3MDL::selfTest(bool flag) {
  write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&flag), 1, 0);
}

/**************************************************************************/
/*!
    @brief Get the magnetic data rate.
    @returns The data rate in float
*/
float Adafruit_LIS3MDL::magneticFieldSampleRate(void) {
  switch (this->getDataRate()) {
  case LIS3MDL_DATARATE_0_625_HZ:
    return 0.625f;
  case LIS3MDL_DATARATE_1_25_HZ:
    return 1.25f;
  case LIS3MDL_DATARATE_2_5_HZ:
    return 2.5f;
  case LIS3MDL_DATARATE_5_HZ:
    return 5.0f;
  case LIS3MDL_DATARATE_10_HZ:
    return 10.0f;
  case LIS3MDL_DATARATE_20_HZ:
    return 20.0f;
  case LIS3MDL_DATARATE_40_HZ:
    return 40.0f;
  case LIS3MDL_DATARATE_80_HZ:
    return 80.0f;
  case LIS3MDL_DATARATE_155_HZ:
    return 155.0f;
  case LIS3MDL_DATARATE_300_HZ:
    return 300.0f;
  case LIS3MDL_DATARATE_560_HZ:
    return 560.0f;
  case LIS3MDL_DATARATE_1000_HZ:
    return 1000.0f;
  }

  return 0;
}

/**************************************************************************/
/*!
    @brief Check for available data from magnetic
    @returns 1 if available, 0 if not
*/
int Adafruit_LIS3MDL::magneticFieldAvailable(void) {
  uint8_t reg_status;
  read_reg(LIS3MDL_REG_STATUS, &reg_status, 1);
  return (reg_status & 0x08) ? 1 : 0;
}

/**************************************************************************/
/*!
    @brief Read magnetic data
    @param x reference to x axis
    @param y reference to y axis
    @param z reference to z axis
    @returns 1 if success, 0 if not
*/
int Adafruit_LIS3MDL::readMagneticField(float &x, float &y, float &z) {
  int16_t data[3];
  HAL_StatusTypeDef res = read_reg(LIS3MDL_REG_OUT_X_L, (uint8_t*)data, 6);
  if (res != HAL_OK) {
    x = y = z = NAN;
    return 0;
  }

  x = data[0] * 4.0 * 100.0 / 32768.0;
  y = data[1] * 4.0 * 100.0 / 32768.0;
  z = data[2] * 4.0 * 100.0 / 32768.0;

  return 1;
}

Adafruit_LIS3MDL::xyz Adafruit_LIS3MDL::get_last_mag() {
	read();
	return Adafruit_LIS3MDL::xyz{x_gauss, y_gauss, z_gauss};
}
