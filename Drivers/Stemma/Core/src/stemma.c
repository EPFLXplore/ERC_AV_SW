#include "stemma.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "main.h"



uint8_t stemma_init(moist_meter *dev, I2C_HandleTypeDef *i2cHandle){

	dev->i2cHandle = i2cHandle;
	dev->temp_C = 0.0f;
	dev->moisture = 0.0f;
	char text[100];
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	status = sw_reset(dev);
//	return status;
	uint8_t regData[2] = {(uint8_t) SEESAW_STATUS_BASE,(uint8_t) SEESAW_STATUS_HW_ID };
	uint8_t res = 43;

	status = HAL_I2C_Master_Transmit(dev->i2cHandle, SEESAW_ADDRESS << 1, &regData, 2, HAL_MAX_DELAY);
	if (status != HAL_OK){
		return 1;
	}
	HAL_Delay(10);
	status = HAL_I2C_Master_Receive(dev->i2cHandle, SEESAW_ADDRESS << 1, &res, 1, HAL_MAX_DELAY);
	if (status != HAL_OK){
		return 1;
	}
	HAL_Delay(10);
	if((res == SEESAW_HW_ID_CODE_SAMD09)||(res == SEESAW_HW_ID_CODE_TINY8X7)){

		errNum += (status != HAL_OK);
		return 0;
	}

	return 1;
}

HAL_StatusTypeDef sw_reset(moist_meter *dev){
	uint8_t status = 0;
	uint8_t regData[3] = {(uint8_t)SEESAW_STATUS_BASE, (uint8_t)SEESAW_STATUS_SWRST, (uint8_t)0xFF };
	status = HAL_I2C_Master_Transmit(dev->i2cHandle, SEESAW_ADDRESS << 1, &regData, 3, HAL_MAX_DELAY);
	HAL_Delay(500);
	return status;
}

HAL_StatusTypeDef stemma_ReadTemp(moist_meter *dev){
	uint8_t buf[4];
	uint8_t status = 0;
	uint8_t regData[2] = {(uint8_t) SEESAW_STATUS_BASE,(uint8_t) SEESAW_STATUS_TEMP };
	HAL_I2C_Master_Transmit(dev->i2cHandle, SEESAW_ADDRESS << 1, &regData, 2, HAL_MAX_DELAY);
	HAL_Delay(5);
	status = HAL_I2C_Master_Receive(dev->i2cHandle, SEESAW_ADDRESS << 1, &buf, 4, HAL_MAX_DELAY);
	HAL_Delay(5);
	int32_t temp = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
					((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
	dev->temp_C = (1.0 / (1UL << 16)) * temp;
	return status;
}

HAL_StatusTypeDef stemma_ReadMoisture(moist_meter *dev, uint8_t pin){
  uint8_t buf[2];
  uint8_t p = pin;
  uint8_t status = 0;
  uint16_t ret = 65535;
  uint8_t regData[2] = {(uint8_t) SEESAW_TOUCH_BASE,(uint8_t) (SEESAW_TOUCH_CHANNEL_OFFSET + p) };

  for (uint8_t retry = 0; retry < 5; retry++) {
	  HAL_I2C_Master_Transmit(dev->i2cHandle, SEESAW_ADDRESS << 1, &regData, 2, HAL_MAX_DELAY);
	  HAL_Delay(10);
	  status = HAL_I2C_Master_Receive(dev->i2cHandle, SEESAW_ADDRESS << 1, &buf, 2, 3000 + retry * 1000);
	  HAL_Delay(10);
	if (status == HAL_OK) {
	  dev->moisture = ((uint16_t)buf[0] << 8) | buf[1];
	  return status;
	}
  }
  return status;
}

HAL_StatusTypeDef stemma_ReadRegister(moist_meter *dev, uint8_t reg, uint8_t *data ) {
	return HAL_I2C_Mem_Read( dev->i2cHandle, SEESAW_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );
}
HAL_StatusTypeDef stemma_ReadRegisters(moist_meter *dev, uint8_t reg, uint8_t *data, uint8_t length){
	return HAL_I2C_Mem_Write( dev->i2cHandle, SEESAW_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY );
}

HAL_StatusTypeDef stemma_WriteRegister(moist_meter *dev, uint8_t reg, uint8_t *data){
	return HAL_I2C_Mem_Write( dev->i2cHandle, SEESAW_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );
}

HAL_StatusTypeDef stemma_WriteRegisters(moist_meter *dev, uint8_t reg, uint8_t *data, uint8_t length){
	return HAL_I2C_Mem_Write( dev->i2cHandle, SEESAW_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY );
}
