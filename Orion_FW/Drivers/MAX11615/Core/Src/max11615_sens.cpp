/*
 * MAX11615.cpp
 *
 *  Created on: May 11, 2023
 *      Author: leo
 */
#include "max11615_sens.h
#include "max11615_sens_defs.h"


bool Max11615_Sens::write_reg(uint8_t reg, uint8_t val, uint16_t len){
	auto res = HAL_I2C_Mem_Write(&(this->i2c), this->dev_addr, reg, I2C_MEMADD_SIZE_8BIT, &val, len, I2C_TIMEOUT);
	return res == HAL_OK;
}

bool Max11615_Sens::read_reg(uint8_t reg, uint8_t val, uint16_t len){

	auto res = HAL_I2C_Mem_Read(&(this->i2c), this->dev_addr, reg, I2C_MEMADD_SIZE_8BIT, &val, len, I2C_TIMEOUT);
	return res == HAL_OK;
}

bool Max11615_Sens::init(){
	uint8_t config_byte = (MAX11615_CONFIG | (this->channel << 1));
	auto res = HAL_I2C_Master_Transmit(&(this->i2c), ADDRESS_MAX11615 << 1, &config_byte, 1, I2C_TIMEOUT);
	return res == HAL_OK;
}




