/*
 * ina219.cpp
 *
 *  Created on: Aug 28, 2022
 *      Author: Vincent
 */

#include "ina219.h"
#include "cmsis_os.h"

// Read the register

HAL_StatusTypeDef Write16(INA219_t *ina219, uint8_t reg, uint16_t value)
{
	uint8_t pData[2];
	pData[0] = (value >> 8);
	pData[1] = value & 0xff;
	return HAL_I2C_Mem_Write(ina219->i2cHandle, (INA219_ADDRESS<<1), reg, 1, pData, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef INA219_setCalibration(INA219_t *ina219, uint16_t CalibrationData)
{
	return Write16(ina219, INA219_REG_CALIBRATION, CalibrationData);
}

HAL_StatusTypeDef INA219_setConfig(INA219_t *ina219, uint16_t Config)
{
	return Write16(ina219, INA219_REG_CONFIG, Config);
}


HAL_StatusTypeDef INA219_Reset(INA219_t *ina219)
{
	HAL_StatusTypeDef status = Write16(ina219, INA219_REG_CONFIG, INA219_CONFIG_RESET);
	osDelay(1);
	return status;
}

HAL_StatusTypeDef INA219_setCalibration_32V_2A(INA219_t *ina219)
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	             INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	             INA219_CONFIG_SADCRES_12BIT_1S_532US |
	             INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219->calibrationValue = 4096;
	ina219->currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
	ina219->powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

	if (INA219_setCalibration(ina219, ina219->calibrationValue) != HAL_OK)
		return HAL_ERROR;
	if (INA219_setConfig(ina219, config) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef INA219_init(INA219_t *ina219, I2C_HandleTypeDef *i2c)
{
	ina219->i2cHandle = i2c;

	ina219->currentDivider_mA = 0;
	ina219->powerMultiplier_mW = 0;


	if(INA219_Reset(ina219) != HAL_OK)
		return HAL_ERROR;
	if(INA219_setCalibration_32V_2A(ina219) != HAL_OK)
		return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef INA219_ReadCurrent_raw(INA219_t *ina219)
{
	uint8_t Value[2];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(ina219->i2cHandle, (INA219_ADDRESS<<1), INA219_REG_CURRENT, 1, Value, 2, 1000);
	ina219->current = ((Value[0] << 8) | Value[1]);
	return status;
}

HAL_StatusTypeDef INA219_ReadCurrent(INA219_t *ina219)
{
	if (INA219_ReadCurrent_raw(ina219) == HAL_OK) {
		ina219->current = (ina219->current)/(ina219->currentDivider_mA);
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

