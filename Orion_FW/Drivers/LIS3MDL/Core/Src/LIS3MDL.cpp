/*
 * LIS3MDL.cpp
 *
 *  Created on: Jul 26, 2023
 *      Author: Vincent Nguyen
 */

#include "LIS3MDL.hpp"
#include <cmsis_os.h>

HAL_StatusTypeDef LIS3MDL::read_reg(uint8_t reg, uint8_t* data, uint16_t len) {
//  not sure if necessary to suspend tasks and disable interrupts
//	vTaskSuspendAll();
//	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Read(this->i2c, this->dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
//	__enable_irq();
//	xTaskResumeAll();
	return res;
}

HAL_StatusTypeDef LIS3MDL::read_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift) {
    if (data == nullptr || bits > 8 || shift > 7) {
        return HAL_ERROR; // Invalid parameters
    }

    uint8_t reg_value;
    HAL_StatusTypeDef res;

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

HAL_StatusTypeDef LIS3MDL::write_reg(uint8_t reg, uint8_t* data, uint16_t len) {
//  not sure if necessary, to test
//	vTaskSuspendAll();
//	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Write(this->i2c, this->dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
//	__enable_irq();
//	xTaskResumeAll();

	return res;
}

HAL_StatusTypeDef LIS3MDL::write_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift) {
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

/**************************************************************************/
/*!

@brief  Initializes magnetometer with given config
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::init() {
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

	res = reset();
	if (res != HAL_OK)
	  return res;

	res = setPerformanceMode(this->conf.perf_conf);
	if (res != HAL_OK)
	  return res;

	res = setDataRate(this->conf.odr_conf);
	if (res != HAL_OK)
	  return res;

	res = setRange(this->conf.range_conf);
	if (res != HAL_OK)
		return res;

	res = setOperationMode(this->conf.op_conf);
	if (res != HAL_OK)
		return res;

	return HAL_OK;
}

/**************************************************************************/
/*!

@brief  Performs a software reset
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::reset(void) {
	HAL_StatusTypeDef res = write_bits(LIS3MDL_REG_CTRL_REG2, (uint8_t*)0x1, 1, 2);
	if (res != HAL_OK)
		return res;

	osDelay(10);
	return updateRange();
}

/**************************************************************************/
/*!
  @brief  Read the XYZ data from the magnetometer and store in the internal
  x, y and z (and x_g, y_g, z_g) member variables.
*/
/**************************************************************************/

HAL_StatusTypeDef LIS3MDL::read(void) {
	uint8_t buffer[6];
	HAL_StatusTypeDef res;
	res = read_reg(LIS3MDL_REG_OUT_X_L, buffer, 6);
	if (res != HAL_OK)
		return res;

	x = buffer[0] | (buffer[1] << 8);
	y = buffer[2] | (buffer[3] << 8);
	z = buffer[4] | (buffer[5] << 8);

	float scale = 1; // LSB per Gauss
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

	// Convert raw values to Gauss units

	x_gauss = (float)x / scale;
	y_gauss = (float)y / scale;
	z_gauss = (float)z / scale;

	// Convert Gauss to uTesla

	x_uT = x_gauss*100;
	y_uT = y_gauss*100;
	z_uT = z_gauss*100;

	// Compensate for hard and soft iron deformation

	float x_nb = x_uT - HARD_IRON[0];
	float y_nb = y_uT - HARD_IRON[1];
	float z_nb = z_uT - HARD_IRON[2];

	x_cal_uT = SOFT_IRON[0][0]*x_nb + SOFT_IRON[0][1]*y_nb + SOFT_IRON[0][2]*z_nb;
	y_cal_uT = SOFT_IRON[1][0]*x_nb + SOFT_IRON[1][1]*y_nb + SOFT_IRON[1][2]*z_nb;
	z_cal_uT = SOFT_IRON[2][0]*x_nb + SOFT_IRON[2][1]*y_nb + SOFT_IRON[2][2]*z_nb;

	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Set the performance mode, LIS3MDL_LOWPOWERMODE, LIS3MDL_MEDIUMMODE,
    LIS3MDL_HIGHMODE or LIS3MDL_ULTRAHIGHMODE
    @param mode Enumerated lis3mdl_performancemode_t
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::setPerformanceMode(lis3mdl_performancemode_t mode) {
	HAL_StatusTypeDef res;
	// write xy
	res = write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&mode), 2, 5);
	if (res != HAL_OK)
		return res;

	// write z
	return write_bits(LIS3MDL_REG_CTRL_REG4, (uint8_t*)(&mode), 2, 2);
}

/**************************************************************************/
/*!
    @brief Get the performance mode
    @param Enumerated lis3mdl_performancemode_t, LIS3MDL_LOWPOWERMODE,
    LIS3MDL_MEDIUMMODE, LIS3MDL_HIGHMODE or LIS3MDL_ULTRAHIGHMODE
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::getPerformanceMode(lis3mdl_performancemode_t& mode) {
	HAL_StatusTypeDef res;
	uint8_t performance;
	res = read_bits(LIS3MDL_REG_CTRL_REG1, &performance, 2, 5);
	if (res != HAL_OK)
		return res;

	mode = (lis3mdl_performancemode_t)performance;
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the LIS3MDL (controls power consumption)
    from 0.625 Hz to 80Hz
    @param dataRate Enumerated lis3mdl_dataRate_t
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::setDataRate(lis3mdl_dataRate_t dataRate) {
	HAL_StatusTypeDef res;
	if (dataRate == LIS3MDL_DATARATE_155_HZ) {
		// set OP to UHP
		res = setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
		if (res != HAL_OK)
			return res;
	}
	if (dataRate == LIS3MDL_DATARATE_300_HZ) {
		// set OP to HP
		res = setPerformanceMode(LIS3MDL_HIGHMODE);
		if (res != HAL_OK)
			return res;
	}
	if (dataRate == LIS3MDL_DATARATE_560_HZ) {
		// set OP to MP
		res = setPerformanceMode(LIS3MDL_MEDIUMMODE);
		if (res != HAL_OK)
			return res;
	}
	if (dataRate == LIS3MDL_DATARATE_1000_HZ) {
		// set OP to LP
		res = setPerformanceMode(LIS3MDL_LOWPOWERMODE);
		if (res != HAL_OK)
			return res;
	}
	osDelay(10);
	return write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&dataRate), 4, 1);
}

/**************************************************************************/
/*!
    @brief  Gets the data rate for the LIS3MDL (controls power consumption)
    @param Enumerated lis3mdl_dataRate_t from 0.625 Hz to 80Hz
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::getDataRate(lis3mdl_dataRate_t& rate) {
	HAL_StatusTypeDef res;
	uint8_t datarate;
	res = read_bits(LIS3MDL_REG_CTRL_REG1, &datarate, 4, 1);
	if (res != HAL_OK)
		return res;

	rate = (lis3mdl_dataRate_t)datarate;
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Set the operation mode, LIS3MDL_CONTINUOUSMODE,
    LIS3MDL_SINGLEMODE or LIS3MDL_POWERDOWNMODE
    @param mode Enumerated lis3mdl_operationmode_t
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::setOperationMode(lis3mdl_operationmode_t mode) {
  // write x and y
  return write_bits(LIS3MDL_REG_CTRL_REG3, (uint8_t*)(&mode), 2, 0);
}


/**************************************************************************/
/*!
    @brief Get the operation mode
    @param Enumerated lis3mdl_operationmode_t, LIS3MDL_CONTINUOUSMODE,
    LIS3MDL_SINGLEMODE or LIS3MDL_POWERDOWNMODE
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::getOperationMode(lis3mdl_operationmode_t& mode) {
	HAL_StatusTypeDef res;
	uint8_t opmode;
	res = read_bits(LIS3MDL_REG_CTRL_REG3, &opmode, 2, 0);
	if (res != HAL_OK)
		return res;

	mode = (lis3mdl_operationmode_t)opmode;
	return HAL_OK;
}


/**************************************************************************/
/*!
    @brief Set the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @param range Enumerated lis3mdl_range_t
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::setRange(lis3mdl_range_t range) {
	HAL_StatusTypeDef res;
	res = write_bits(LIS3MDL_REG_CTRL_REG2, (uint8_t*)(&range), 2, 5);
	if (res != HAL_OK)
		return res;

	rangeBuffered = range;
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Read the resolution range: +-4 gauss, 8 gauss, 12 gauss, or 16 gauss.
    @param Enumerated lis3mdl_range_t
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::getRange(lis3mdl_range_t& range) {
	HAL_StatusTypeDef res;
	uint8_t read_range;
	res = read_bits(LIS3MDL_REG_CTRL_REG2, &read_range, 2, 5);
	if (res != HAL_OK)
		return res;

	rangeBuffered = (lis3mdl_range_t)read_range;
	range = rangeBuffered;
	return HAL_OK;
}

HAL_StatusTypeDef LIS3MDL::updateRange(void) {
	uint8_t range;
	HAL_StatusTypeDef res = read_bits(LIS3MDL_REG_CTRL_REG2, &range, 2, 5);
	if (res != HAL_OK)
		return res;
	rangeBuffered = (lis3mdl_range_t)range;
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Set the interrupt threshold value
    @param value 16-bit unsigned raw value
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::setIntThreshold(uint16_t value) {
	value &= 0x7FFF; // high bit must be 0!
	return write_reg(LIS3MDL_REG_INT_THS_L, (uint8_t*)value, 2);
}

/**************************************************************************/
/*!
    @brief Get the interrupt threshold value
    @param 16-bit unsigned raw value
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::getIntThreshold(uint16_t& value) {
	HAL_StatusTypeDef res;
	uint16_t int_threshold;
	res = read_reg(LIS3MDL_REG_INT_THS_L, (uint8_t*)&int_threshold, 2);
	if (res != HAL_OK)
		return res;

	value = int_threshold;
	return HAL_OK;
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
HAL_StatusTypeDef LIS3MDL::configInterrupt(bool enableX, bool enableY, bool enableZ,
                                       bool polarity, bool latch,
                                       bool enableInt) {
	uint8_t value = 0x08; // set default bits, see table 36
	value |= enableX << 7;
	value |= enableY << 6;
	value |= enableZ << 5;
	value |= polarity << 2;
	value |= latch << 1;
	value |= enableInt;

	return write_reg(LIS3MDL_REG_INT_CFG, &value, 1);
}

/**************************************************************************/
/*!
    @brief Enable or disable self-test
    @param flag If true, enable self-test

*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::selfTest(bool flag) {
	return write_bits(LIS3MDL_REG_CTRL_REG1, (uint8_t*)(&flag), 1, 0);
}

/**************************************************************************/
/*!
    @brief Get the magnetic data rate.
    @param The data rate in float
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::magneticFieldSampleRate(float& rate) {
	lis3mdl_dataRate_t datarate;
	HAL_StatusTypeDef res = getDataRate(datarate);
	if (res != HAL_OK)
		return res;
	switch (datarate) {
		case LIS3MDL_DATARATE_0_625_HZ:
			rate = 0.625f;
			break;
		case LIS3MDL_DATARATE_1_25_HZ:
			rate = 1.25f;
			break;
		case LIS3MDL_DATARATE_2_5_HZ:
			rate = 2.5f;
			break;
		case LIS3MDL_DATARATE_5_HZ:
			rate = 5.0f;
			break;
		case LIS3MDL_DATARATE_10_HZ:
			rate = 10.0f;
			break;
		case LIS3MDL_DATARATE_20_HZ:
			rate = 20.0f;
			break;
		case LIS3MDL_DATARATE_40_HZ:
			rate = 40.0f;
			break;
		case LIS3MDL_DATARATE_80_HZ:
			rate = 80.0f;
			break;
		case LIS3MDL_DATARATE_155_HZ:
			rate = 155.0f;
			break;
		case LIS3MDL_DATARATE_300_HZ:
			rate = 300.0f;
			break;
		case LIS3MDL_DATARATE_560_HZ:
			rate = 560.0f;
			break;
		case LIS3MDL_DATARATE_1000_HZ:
			rate = 1000.0f;
			break;
	}
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Check for available data from magnetic
    @param true if available, false if not
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::magneticFieldAvailable(bool& available) {
	HAL_StatusTypeDef res;
	uint8_t reg_status;
	res = read_reg(LIS3MDL_REG_STATUS, &reg_status, 1);
	if (res != HAL_OK)
		return res;

	available = (reg_status & 0x08) ? true : false;
	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief Read magnetic data
    @param x reference to x axis
    @param y reference to y axis
    @param z reference to z axis
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::readMagneticField(float &x_, float &y_, float &z_) {
	int16_t data[3];
	HAL_StatusTypeDef res = read_reg(LIS3MDL_REG_OUT_X_L, (uint8_t*)data, 6);
	if (res != HAL_OK) {
		x = y = z = NAN;
		return res;
	}

	x = data[0] * 4.0 * 100.0 / 32768.0;
	y = data[1] * 4.0 * 100.0 / 32768.0;
	z = data[2] * 4.0 * 100.0 / 32768.0;

	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief get uncalibrated magnetic readings in uTesla
    @param uncalibrated magnetic readings in uTesla
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::get_mag(Vector& mag) {
	HAL_StatusTypeDef res = read();
	if (res != HAL_OK)
		return res;

	mag.x = x_uT;
	mag.y = y_uT;
	mag.z = z_uT;

	return HAL_OK;
}

/**************************************************************************/
/*!
    @brief calibrated magnetic readings in uTesla
    @param calibrated magnetic readings in uTesla
*/
/**************************************************************************/
HAL_StatusTypeDef LIS3MDL::get_mag_cal(Vector& mag) {
	HAL_StatusTypeDef res = read();
	if (res != HAL_OK)
		return res;

	mag.x = x_cal_uT;
	mag.y = y_cal_uT;
	mag.z = z_cal_uT;

	return HAL_OK;
}
