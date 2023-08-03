/*
 * ADS1115_ADS1115.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Adafruit. Rewritten by Daniel Mårtensson
 */

#ifndef SRC_ADS1115_ADS1115_ADS1115_ADS1115_H_
#define SRC_ADS1115_ADS1115_ADS1115_ADS1115_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "ADS1115_defines.h"

class ADS1115 {
public:
	ADS1115(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress);
	HAL_StatusTypeDef ADS1115_init();
	HAL_StatusTypeDef ADSbegin();
	HAL_StatusTypeDef ADSreadADC_SingleEnded(uint8_t channel, uint16_t& val);
	HAL_StatusTypeDef  ADSreadADC_Differential_0_1(int16_t& val);
	HAL_StatusTypeDef  ADSstartComparator_SingleEnded(uint8_t channel, int16_t threshold);
	HAL_StatusTypeDef  ADSgetLastConversionResults(int16_t& val);
	void ADSsetGain(adsGain_t gain);
	adsGain_t  ADSgetGain();
	float get_full_scale();

	void set_scale(uint8_t channel, float scale);
	float get_scale (uint8_t channel);
	void set_offset(uint8_t channel, float scale);
	float get_offset(uint8_t channel);

	HAL_StatusTypeDef read_average(uint8_t channel, float& val, uint16_t times = 10);
	HAL_StatusTypeDef get_value_offset(uint8_t channel, float& val, uint16_t times = 10);
	HAL_StatusTypeDef get_value_conv(uint8_t channel, float& val, uint16_t times = 1);
private:
	HAL_StatusTypeDef writeRegister(int8_t reg, uint16_t value);
	HAL_StatusTypeDef readRegister(uint8_t reg, uint16_t& val);

	uint16_t m_i2cAddress;      ///< the I2C address
	uint32_t m_conversionDelay; ///< conversion delay
	uint8_t m_bitShift;        ///< bit shift amount
	float full_scale;
	adsGain_t m_gain;          ///< ADC gain
	uint16_t m_dataRate;
	I2C_HandleTypeDef* hi2c;    // Handle for I2C

	float OFFSET[4] = {0, 0, 0, 0};
	float SCALE[4] = {1,1,1,1};
};

#endif /* SRC_ADS1115_ADS1115_ADS1115_ADS1115_H_ */
